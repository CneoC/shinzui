#include "Core.h"
#include "Process.h"

//Log<LogOutputFile, LogOutputText> CoreLog();

Core::Core(s32 threadCount)
	: m_startTime(Time::NOW)
	, m_threadCount(threadCount)
	, m_processRunId(0)
{
	// Determine thread count automatically
	if (m_threadCount == -1)
	{
		m_threadCount = boost::thread::hardware_concurrency();
	}

	// Limit to 31 threads
	if (m_threadCount > 31) m_threadCount = 31;

	// Create the thread pool
	for (u32 i = 1; i <= m_threadCount; i++)
	{
		m_threads.add_thread(new boost::thread(Thread(), this, 1 << i));
	}
}

Core::~Core()
{
}

//////////////////////////////////////////////////////////////////////////

void Core::run()
{
	// Run the main thread processes
	m_mainThread(this, -1);
}

//////////////////////////////////////////////////////////////////////////

Process *Core::getNextProcess(u32 threadId)
{
	boost::mutex::scoped_lock lock(m_processesMutex);
	ProcessList::iterator p = m_processes.begin();
	while (p != m_processes.end())
	{
		Process *pProcess = *p;
		// Do target thread id's match?
		if (pProcess->getTargetThreadId() & threadId)
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

Process *Core::findProcess(u32 id)
{
	boost::mutex::scoped_lock lock(m_processesMutex);

	ProcessList::iterator i = m_processes.begin();
	while (i != m_processes.end())
	{
		if ((*i)->getId() == id)
		{
			return *i;
		}
		i++;
	}
	return NULL;
}

void Core::Thread::operator () (Core *pCore, u32 threadId)
{
	while (pCore->isRunning())
	{
		Process *pProcess = pCore->getNextProcess(threadId);
		if (pProcess)
		{
			double elapsedTime = pCore->getElapsedTime();
			double delta = elapsedTime - pProcess->getLastRunTime();

			pProcess->setLastRunTime(elapsedTime);
			pProcess = pProcess->run(delta);
			if (pProcess) pCore->addProcess(pProcess);
		}
		boost::thread::yield();
	}
}
