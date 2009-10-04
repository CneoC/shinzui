#include "Core.h"
#include "Process.h"

using namespace core;

//////////////////////////////////////////////////////////////////////////

Core::Core(s32 threadCount)
	: m_startTime(os::Time::NOW)
	, m_processRunId(0)
	, m_log(LOG_GET("Core"))
	, m_job(0, NULL)
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

	while (isRunning())
	{
		// Run a core process if one is available
		m_job = getNextJob(THREAD_ID_CORE_BIT);
		if (m_job.second)
			runJob(m_job);
		
		// Copy waiting processes that have passed their frame delay to the main processes list.		
		{
			double elapsed = getElapsedTime();
			boost::lock_guard<boost::shared_mutex> lock(m_waitingProcessesMutex);

			// Don't bother adding more processes than we can handle
			u32 max = m_threads.size() - m_processes.size();

			// For all waiting processes
			ProcessList::iterator p = m_waitingProcesses.begin();
			while (p != m_waitingProcesses.end() && max > 0)
			{
				Process *pProcess = *p;

				// If the process wait is over
				double delta = elapsed - pProcess->getLastRunTime();
				if (delta >= pProcess->getFrameDelay())
				{
					boost::lock_guard<boost::shared_mutex> lock(m_processesMutex);

					// Reset job counters
					pProcess->resetJobs();

					// Change from waiting to active list
					m_processes.push_back(pProcess);
					p = m_waitingProcesses.erase(p);

					--max;
				}
				else
				{
					break;
				}
			}
		}

		// Assign processes to free running threads
		ThreadList::iterator iter;
		for (iter = m_threads.begin(); iter != m_threads.end(); ++iter)
		{
			CoreThread *pThread = *iter;
			if (!pThread->getJob().second)
			{
				CoreJob job = getNextJob(pThread->getId());
				if (job.second)
				{
					{
						boost::lock_guard<boost::mutex> lock(pThread->getProcessMutex());
						pThread->setJob(job);
					}
					// Notify the waiting thread it has a job
					pThread->getCondition().notify_one();
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

// TODO: optimize
CoreJob Core::getNextJob(u32 threadMask)
{
	boost::lock_guard<boost::shared_mutex> lock(m_processesMutex);

	double elapsed = getElapsedTime();

	// For all active processes
	ProcessList::iterator p = m_processes.begin();
	while (p != m_processes.end())
	{
		Process *pProcess = *p;

		// Do target thread id's match?
		u32 processThreadMask = pProcess->getActiveJobs() == 0? pProcess->getThreadMask(): pProcess->getJobThreadMask();
		if (processThreadMask & threadMask)
		{
			// Are it's dependencies done, getFrameDelay is already checked in Core thread
			if (pProcess->isDependencyDone())
			{
				if (LOG_CHECK(m_log, LEVEL_WARN))
				{
					double diff = elapsed - pProcess->getNextRunTime();
					if (diff > 0.1) LOG_WARN(m_log, "Process expected execution time exceeded by: " << diff);
				}

				// Increase job count for the process
				u16 jobs = pProcess->incActiveJobs();
				if (jobs == 1)
					pProcess->setLastRunId(m_processRunId++);

				// If all requested jobs have been activated, remove the process from the active list.
				if (jobs >= pProcess->getJobs())
				{
					if (jobs > pProcess->getJobs())
						LOG_WARN(m_log, LOG_FMT("More active jobs (%i) than expected (%i) for: %X", jobs % pProcess->getJobs() % pProcess));
					m_processes.erase(p);
				}

				return CoreJob(jobs - 1, pProcess);
			}
		}
		++p;
	}
	return CoreJob(0, NULL);
}

// TODO: optimization point - threading
void Core::addProcess(Process *pProcess)
{
	boost::lock_guard<boost::shared_mutex> lock(m_waitingProcessesMutex);

	// TODO: use different sort method?

	if (m_waitingProcesses.empty())
	{
		m_waitingProcesses.push_front(pProcess);
		return;
	}

	// pick side to start insertion sort at
	double nextRun = pProcess->getNextRunTime();
	double frontTime = m_waitingProcesses.front()->getNextRunTime();
	double backTime = m_waitingProcesses.back()->getNextRunTime();
	double halfLength = (backTime - frontTime) / 2;

	// Start insertion sort from first entry
	if (nextRun - frontTime <= halfLength)
	{
		ProcessList::iterator p = m_waitingProcesses.begin();
		while (p != m_waitingProcesses.end())
		{
			if (nextRun < (*p)->getNextRunTime())
			{
				m_waitingProcesses.insert(p, pProcess);
				return;
			}
			++p;
		}

		// Process not added yet, so put it in the back
		m_waitingProcesses.push_back(pProcess);
	}
	// Start insertion sort from last entry
	else
	{
		ProcessList::reverse_iterator p = m_waitingProcesses.rbegin();
		while (p != m_waitingProcesses.rend())
		{
			if (nextRun >= (*p)->getNextRunTime())
			{
				m_waitingProcesses.insert(p.base(), pProcess);
				return;
			}
			++p;
		}

		// Process not added yet, so put it in the front
		m_waitingProcesses.push_front(pProcess);
	}
}

void Core::removeProcess(Process *pProcess)
{
	{
		boost::lock_guard<boost::shared_mutex> lock(m_processesMutex);
		m_processes.remove(pProcess);
	}
	{
		boost::lock_guard<boost::shared_mutex> lock(m_waitingProcessesMutex);
		m_waitingProcesses.remove(pProcess);
	}
}

void Core::runJob(const CoreJob &job)
{
	double elapsedTime	= getElapsedTime();
	double delta		= elapsedTime - job.second->getLastRunTime();

	// Initialize process on first run
	if (job.first == 0 && job.second->getLastRunTime() == 0)
		job.second->init();
	Process *pAdd = job.second->run(job.first, delta);

	// If we have a new process to add, and all process jobs have finished (this being the last one)
	u16 jobs = job.second->incFinishedJobs();
	if (pAdd && jobs >= job.second->getJobs())
	{
		job.second->setLastRunTime(elapsedTime);
		if (jobs > job.second->getJobs())
			LOG_WARN(getLog(), LOG_FMT("More finished jobs (%i) than expected (%i) for: %X", jobs % job.second->getJobs() % job.second));
		addProcess(pAdd);
	}

}

//////////////////////////////////////////////////////////////////////////

bool CoreThread::run()
{
	boost::unique_lock<boost::mutex> lock(m_mutex);
	while (!m_job.second && m_pCore->isRunning())
	{
		m_condition.wait(lock);
	}

	if (m_job.second != NULL)
	{
		m_pCore->runJob(m_job);
		m_job = CoreJob(0, NULL);
	}

	return m_pCore->isRunning();
}
