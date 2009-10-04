#include "Core.h"
#include "Process.h"

using namespace core;

//////////////////////////////////////////////////////////////////////////

Core::Core(s32 threadCount)
	: m_startTime(os::Time::NOW)
	, m_processRunId(0)
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

	while (isRunning())
	{
		// Run a core process if one is available
		Process *pProcess = getNextProcess(THREAD_ID_CORE_BIT);
		if (pProcess)
		{
			double elapsedTime	= getElapsedTime();
			double delta		= elapsedTime - pProcess->getLastRunTime();

			if (pProcess->getLastRunTime() == 0)
				pProcess->init();
			pProcess->setLastRunTime(elapsedTime);
			Process *pAddProcess = pProcess->run(delta);
			if (pAddProcess)
				addProcess(pAddProcess);
		}

		// Copy waiting processes that have passed their frame delay to the main processes list.		
		{
			double elapsed = getElapsedTime();
			boost::lock_guard<boost::shared_mutex> lock(m_waitingProcessesMutex);

			// Don't bother adding more processes than we can handle
			u32 max = m_threads.size() - m_processes.size();

			ProcessList::iterator p = m_waitingProcesses.begin();
			while (p != m_waitingProcesses.end() && max > 0)
			{
				pProcess = *p;
				double delta = elapsed - pProcess->getLastRunTime();
				if (delta >= pProcess->getFrameDelay())
				{
					boost::lock_guard<boost::shared_mutex> lock(m_processesMutex);

					// Reset job amounts
					pProcess->resetJobs();
					m_processes.push_back(pProcess);

					// No longer in waiting list
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
			if (!pThread->getProcess())
			{
				pProcess = getNextProcess(pThread->getId());
				if (pProcess)
				{
					{
						boost::lock_guard<boost::mutex> lock(pThread->getProcessMutex());
						pThread->setProcess(pProcess);
					}
					pThread->getCondition().notify_all();
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

// TODO: optimize
Process *Core::getNextProcess(u32 threadMask)
{
	boost::lock_guard<boost::shared_mutex> lock(m_processesMutex);

	double elapsed = getElapsedTime();

	ProcessList::iterator p = m_processes.begin();
	while (p != m_processes.end())
	{
		Process *pProcess = *p;

		// Do target thread id's match?
		if (pProcess->getTargetThreadMask() & threadMask)
		{
			double delta = elapsed - pProcess->getLastRunTime();
			if (delta > pProcess->getFrameDelay() && pProcess->isDependencyDone())
			{
				if (LOG_CHECK(m_log, LEVEL_WARN))
				{
					double diff = elapsed - pProcess->getNextRunTime();
					if (diff > 0.1) LOG_WARN(m_log, "Process expected execution time exceeded by: " << diff);
				}
				// If all requested jobs have been activated, remove te process from the active list.
				u16 jobs = pProcess->incActiveJobs();
				if (jobs == 1)
					pProcess->setLastRunId(m_processRunId++);

				if (jobs >= pProcess->getJobs())
				{
					if (jobs > pProcess->getJobs())
						LOG_WARN(m_log, LOG_FMT("More active jobs (%i) than expected (%i) for: %X", jobs % pProcess->getJobs() % pProcess));
					m_processes.erase(p);
				}
				return pProcess;
			}
		}
		++p;
	}
	return NULL;
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

//////////////////////////////////////////////////////////////////////////

bool CoreThread::run()
{
	m_active = false;
	boost::unique_lock<boost::mutex> lock(m_mutex);
	while (!m_pProcess && m_pCore->isRunning())
	{
		m_condition.wait(lock);
	}

	if (m_pProcess)
	{
		m_active = true;

		double elapsedTime	= m_pCore->getElapsedTime();
		double delta		= elapsedTime - m_pProcess->getLastRunTime();

		// Initialize process on first run
		if (m_pProcess->getLastRunTime() == 0)
			m_pProcess->init();
		m_pProcess->setLastRunTime(elapsedTime);
		Process *pAdd = m_pProcess->run(delta);

		// If we have a new process to add, and all process jobs have finished (this being the last one)
		u16 jobs = m_pProcess->incFinishedJobs();
		if (pAdd && jobs >= m_pProcess->getJobs())
		{
			if (jobs > m_pProcess->getJobs())
				LOG_WARN(m_pCore->getLog(), LOG_FMT("More finished jobs (%i) than expected (%i) for: %X", jobs % m_pProcess->getJobs() % m_pProcess));
			m_pCore->addProcess(pAdd);
		}

		m_pProcess = NULL;
	}

	return m_pCore->isRunning();
}
