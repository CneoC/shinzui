#pragma once

#ifndef __CORE_H__
#define __CORE_H__

#include <boost/thread.hpp>

#include <cassert>

#include "types.h"

#include "util/Log.h"
#include "core/Time.h"

class Process;
class ResourceLoader;

/**
 * Core process handling class.
 * Handles a number of core engine processes possibly using threading.
 */
class Core
{
public:
	typedef std::list<Process *> ProcessList;

	/**
	 * Thread type identifiers.
	 */
	enum ThreadID
	{
		THREAD_ID_NONE		= 0x00000000,
		THREAD_ID_MAIN		= 0x00000001,
		THREAD_ID_NORMAL	= 0xFFFFFFFE,
	};

	/**
	 * Simple core thread class which calls the next queued process.
	 */
	class Thread
	{
	public:
		void operator () (Core *pCore, u32 threadId);
	};

	/**
	 * Core process handling class
	 * @param threadCount	number of threads used for processes, -1 is recommended (2 * #cpu's)
	 */
	Core(s32 threadCount = -1);
	~Core();

	//////////////////////////////////////////////////////////////////////////

	/**
	 * Runs the core engine processes.
	 * This is a blocking function!
	 */
	void run();

	//////////////////////////////////////////////////////////////////////////

	/**
	 * Gets the next process scheduled to run for a specific thread identifier.
	 */
	Process *getNextProcess(u32 threadId);

	/**
	 * Adds a process to the execution list.
	 * @param pProcess	the process to add.
	 */
	void addProcess(Process *pProcess)
	{
		boost::mutex::scoped_lock lock(m_processesMutex);
		m_processes.push_back(pProcess);
	}

	/**
	 * Adds a process from the execution list.
	 * @param pProcess	the process to remove.
	 */
	void removeProcess(Process *pProcess)
	{
		boost::mutex::scoped_lock lock(m_processesMutex);
		m_processes.remove(pProcess);
	}

	/**
	 * Finds a process in the execution list using it's identifier.
	 * @param id	the process identifier to find.
	 */
	Process *findProcess(u32 id);

	/**
	 * Returns the elapsed time since the core system started.
	 * @return elapsed time in seconds.
	 */
	double getElapsedTime() const
	{
		Time time(Time::NOW);
		return (time - m_startTime).getSeconds();
	}

	bool isRunning() const						{ return true; }

	ResourceLoader *getLoader() const			{ return m_pLoader; }
	void setLoader(ResourceLoader *pLoader)		{ m_pLoader = pLoader; }

protected:
	Time					m_startTime;			// start time of the core system.

	u32						m_threadCount;
	Thread					m_mainThread;			// runs application main thread processes.
	boost::thread_group		m_threads;				// pool of threads (can be empty).

	boost::mutex			m_processesMutex;		// mutex used to lock list of processes.
	ProcessList				m_processes;			// list of processes.
	u32						m_processRunId;			// incremental identifier to determine process run order.

	ResourceLoader*			m_pLoader;				// resource loader for access to resources.
};

// Log used for Core and related classes
//extern Log<LogFormatText, LogOutputFile> CoreLog;

#endif //__CORE_H__