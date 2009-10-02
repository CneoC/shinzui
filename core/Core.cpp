#include "Core.h"
#include "Process.h"

Core::Core(s32 threadCount)
	: m_startTime(Time::NOW)
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
			ProcessList::iterator p = m_waitingProcesses.begin();
			while (p != m_waitingProcesses.end())
			{
				pProcess = *p;
				double delta = elapsed - pProcess->getLastRunTime();
				if (delta >= pProcess->getFrameDelay())
				{
					boost::lock_guard<boost::shared_mutex> lock(m_processesMutex);
					m_processes.push_back(pProcess);
					p = m_waitingProcesses.erase(p);
				}
				else
				{
					break;
				}
			}
		}

		// Handle load balancing of running threads
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

		boost::this_thread::yield();
	}
}

//////////////////////////////////////////////////////////////////////////

// TODO: optimization point - threading
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

				m_processes.erase(p);
				pProcess->setLastRunId(m_processRunId++);
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

	// TODO: don't use insertion sort?

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
	boost::unique_lock<boost::mutex> lock(m_mutex);
	while (!m_pProcess && m_pCore->isRunning())
	{
		m_condition.wait(lock);
	}

	if (m_pProcess)
	{
		double elapsedTime	= m_pCore->getElapsedTime();
		double delta		= elapsedTime - m_pProcess->getLastRunTime();

		if (m_pProcess->getLastRunTime() == 0)
			m_pProcess->init();
		m_pProcess->setLastRunTime(elapsedTime);
		Process *pAddProcess = m_pProcess->run(delta);
		if (pAddProcess)
			m_pCore->addProcess(pAddProcess);

		m_pProcess = NULL;
	}

	return m_pCore->isRunning();
}
