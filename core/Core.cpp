#include "Core.h"
#include "Process.h"

#include "boost/shared_ptr.hpp"
using namespace core;

//////////////////////////////////////////////////////////////////////////

Core::Core(s32 threadCount)
	: m_processRunId(0)
	, m_log(LOG_GET("Core"))
{
	// Determine thread count automatically
	if (threadCount == -1)
	{
		threadCount = boost::thread::hardware_concurrency();
	}

	// Clamp thread count
	if (threadCount < 1)	threadCount = 1;
	if (threadCount > 31)	threadCount = 31;

	// Create the thread pool
	m_threads.resize(threadCount);
	for (s32 i = 0; i < threadCount; i++)
	{
		CoreThread *pThread = new CoreThread(this, 1 << i);
		pThread->start();
		m_threads[i] = pThread;
	}
}

Core::~Core()
{
	ThreadList::iterator iter;
	for (iter = m_threads.begin(); iter != m_threads.end(); ++iter)
	{
		CoreThread *pThread = *iter;
		pThread->getCondition().notify_all();
		pThread->stop(true);
		delete pThread;
	}
}

//////////////////////////////////////////////////////////////////////////

void Core::run()
{
	s32 targetThread = 0;
	
	m_startTime = os::Time(os::Time::NOW);

	while (isRunning())
	{
		// Run a core process if one is available
		if (getNextJob(THREAD_ID_CORE_BIT, m_job))
		{
			runJob(m_job);
			m_job.threadMask	= 0;
			m_job.pProcess		= NULL;
		}
		
		// Check if any jobs need to get executed.
		{
			double elapsed = getElapsedTime();
			boost::lock_guard<boost::shared_mutex> lock(m_processesMutex);

			// Don't bother adding more processes than we can handle
			s32 max = m_threads.size() - m_jobs.size();

			// For all processes
			ProcessList::iterator p = m_processes.begin();
			while (p != m_processes.end() && max > 0)
			{
				Process *pProcess = *p;

				// If the process is ready
				double delta = elapsed - pProcess->getLastRunTime();
				if (pProcess->getJobs() == 0 &&		// No longer running any jobs
					delta >= pProcess->getFrameDelay() &&	// Passed expected frame delay between calls
					pProcess->isDependencyDone())			// Dependencies done as well
				{
					// Reset job counters
					pProcess->resetJobs();

					// Set new run identifier (used for dependency checking)
					pProcess->setLastRunId(m_processRunId++);

					if (LOG_CHECK(m_log, LEVEL_WARN))
					{
						double diff = elapsed - pProcess->getNextRunTime();
						if (fabs(diff) > 0.05) LOG_WARN(m_log, "Process expected execution time exceeded by: " << diff);
					}

					pProcess->setDeltaTime(elapsed - pProcess->getLastRunTime());
					pProcess->onStart();
					pProcess->setLastRunTime(elapsed);

					--max;
				}

				++p;
			}
		}

		// Assign jobs to free running threads
		ThreadList::iterator iter;
		Job job;
		for (iter = m_threads.begin(); iter != m_threads.end(); ++iter)
		{
			CoreThread *pThread = *iter;

			// Only assign jobs to inactive threads
			if (!pThread->isWorking())
			{
				if (getNextJob(pThread->getId(), job))
				{
					{
						boost::lock_guard<boost::mutex> lock(pThread->getProcessMutex());
						pThread->setJob(job);
					}
					// Notify the waiting thread it has a job to do
					pThread->getCondition().notify_one();
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

bool Core::getNextJob(u32 threadMask, Job &nextJob)
{
	boost::lock_guard<boost::shared_mutex> lock(m_jobsMutex);

	// For all active processes
	JobList::iterator jobIter = m_jobs.begin();
	while (jobIter != m_jobs.end())
	{
		const Job &job = *jobIter;
		// If the thread masks match
		if (job.threadMask & threadMask)
		{
			nextJob = job;
			m_jobs.erase(jobIter);
			return true;
		}
		++jobIter;
	}

	return false;
}

// TODO: optimization point - threading
void Core::addProcess(Process *pProcess)
{
	boost::lock_guard<boost::shared_mutex> lock(m_processesMutex);

	// TODO: use different sort method?

	if (m_processes.empty())
	{
		m_processes.push_front(pProcess);
		return;
	}

	// pick side to start insertion sort at
	double nextRun = pProcess->getNextRunTime();
	double frontTime = m_processes.front()->getNextRunTime();
	double backTime = m_processes.back()->getNextRunTime();
	double halfLength = (backTime - frontTime) / 2;

	// Start insertion sort from first entry
	if (nextRun - frontTime <= halfLength)
	{
		ProcessList::iterator p = m_processes.begin();
		while (p != m_processes.end())
		{
			if (nextRun < (*p)->getNextRunTime())
			{
				m_processes.insert(p, pProcess);
				return;
			}
			++p;
		}

		// Process not added yet, so put it in the back
		m_processes.push_back(pProcess);
	}
	// Start insertion sort from last entry
	else
	{
		ProcessList::reverse_iterator p = m_processes.rbegin();
		while (p != m_processes.rend())
		{
			if (nextRun >= (*p)->getNextRunTime())
			{
				m_processes.insert(p.base(), pProcess);
				return;
			}
			++p;
		}

		// Process not added yet, so put it in the front
		m_processes.push_front(pProcess);
	}
}

void Core::removeProcess(Process *pProcess)
{
	{
		boost::lock_guard<boost::shared_mutex> lock(m_jobsMutex);

		JobList::iterator job = m_jobs.begin();
		while (job != m_jobs.end())
		{
			if (job->pProcess == pProcess)
			{
				m_jobs.erase(job);
			}
		}
		// TODO: Wait for any threads with jobs for pProcess;
	}
	{
		boost::lock_guard<boost::shared_mutex> lock(m_processesMutex);
		m_processes.remove(pProcess);
	}
}

//////////////////////////////////////////////////////////////////////////

void Core::addJob(Process *pProcess, const Job::Function &func, u32 threadMask)
{
	Job job;
	job.threadMask = threadMask;
	job.pProcess = pProcess;
	job.func = func;

	pProcess->incJobs();

	boost::lock_guard<boost::shared_mutex> lock(m_jobsMutex);
	m_jobs.push_back(job);
}

void Core::runJob(const Job &job)
{
	// Run the job and check if should be added to the job list again.
	if (job.func())
	{
		boost::lock_guard<boost::shared_mutex> lock(m_jobsMutex);
		m_jobs.push_back(job);
	}
	else
	{
		job.pProcess->decJobs();
		if (job.pProcess->getJobs() == 0)
			job.pProcess->onStop();
	}
}

//////////////////////////////////////////////////////////////////////////

bool CoreThread::run()
{
	boost::unique_lock<boost::mutex> lock(m_mutex);
	while (!isWorking() && m_pCore->isRunning())
	{
		m_condition.wait(lock);
	}

	m_pCore->runJob(m_job);
	m_job.threadMask	= 0;
	m_job.pProcess		= NULL;
	++m_jobsDone;

	return m_pCore->isRunning();
}
