#include "Core.h"
#include "Process.h"

//Log<LogOutputFile, LogOutputText> CoreLog();

Core::Core(s32 threadCount)
	: m_startTime(Time::NOW)
	, m_processRunId(0)
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

			pProcess->setLastRunTime(elapsedTime);
			Process *pAddProcess = pProcess->run(delta);
			if (pAddProcess)
				addProcess(pAddProcess);
		}

		boost::this_thread::yield();

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

Process *Core::getNextProcess(u32 threadMask)
{
	boost::lock_guard<boost::shared_mutex> lock(m_processesMutex);

	ProcessList::iterator p = m_processes.begin();
	while (p != m_processes.end())
	{
		Process *pProcess = *p;

		// Do target thread id's match?
		if (pProcess->getTargetThreadMask() & threadMask)
		{
			double delta = getElapsedTime() - pProcess->getLastRunTime();
			if (pProcess->isReady(delta))
			{
				m_processes.erase(p);
				pProcess->setLastRunId(m_processRunId++);
				return pProcess;
			}
		}
		++p;
	}
	return NULL;
}

Process *Core::getProcess(u32 processId)
{
	boost::shared_lock<boost::shared_mutex> lock(m_processesMutex);

	ProcessList::iterator i = m_processes.begin();
	while (i != m_processes.end())
	{
		if ((*i)->getId() == processId)
		{
			return *i;
		}
		i++;
	}
	return NULL;
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

		m_pProcess->setLastRunTime(elapsedTime);
		Process *pAddProcess = m_pProcess->run(delta);
		if (pAddProcess)
			m_pCore->addProcess(pAddProcess);

		m_pProcess = NULL;
	}

	return m_pCore->isRunning();
}
