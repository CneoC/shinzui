#pragma once

#ifndef __CORE_CORE_H__
#define __CORE_CORE_H__

#include <cassert>

#include "types.h"

#include "util/logging/LogManager.h"
#include "os/current/Thread.h"
#include "os/current/Time.h"

class ResourceLoader;
namespace render
{
	class RenderDriver;
}

namespace core
{
	class Process;
	class Core;

	typedef std::pair<u32, Process *>	CoreJob;

	/**
	* Core thread class which is used by Core to run a Process.
	*/
	class CoreThread
		: public os::Thread
	{
	public:
		/**
		* Constructs a thread for use in Core.
		* @param pCore		core class this thread belongs to.
		* @param threadId	thread identifier for this core thread.
		*/
		CoreThread(Core *pCore, u32 id)
			: m_pCore(pCore)
			, m_id(id)
			, m_job(0, NULL)
		{
		}

		virtual bool run();

		Core *getCore() const						{ return m_pCore; }

		u32 getId() const							{ return m_id; }

		void setJob(const CoreJob &job)				{ m_job = job; }
		const CoreJob &getJob() const				{ return m_job; }

		boost::mutex &getProcessMutex()				{ return m_mutex; }
		boost::condition_variable &getCondition()	{ return m_condition; }

		bool isWorking() const						{ return m_job.second != NULL; }

	protected:
		Core *	m_pCore;		// parent core class.
		u32		m_id;			// thread identifier bit.

		boost::condition_variable	m_condition;
		boost::mutex				m_mutex;
		
		CoreJob						m_job;
	};

	/**
	* Core process handling class.
	* Handles a number of core engine processes possibly using threading.
	*/
	class Core
	{
	public:
		typedef std::list<Process *>		ProcessList;
		typedef std::vector<CoreThread *>	ThreadList;

		/**
		* Thread type identifiers.
		*/
		enum ThreadID
		{
			THREAD_ID_MAIN,
			THREAD_ID_LOAD,
			THREAD_ID_NORMAL_START,

			// Thread mask bits
			THREAD_ID_MAIN_BIT			= 1 << THREAD_ID_MAIN,
			THREAD_ID_LOAD_BIT			= 1 << THREAD_ID_LOAD,
			THREAD_ID_NORMAL_START_BIT	= 1 << THREAD_ID_NORMAL_START,
			THREAD_ID_CORE_BIT			= 0x80000000,

			THREAD_ID_NORMAL_MASK		= ~(THREAD_ID_NORMAL_START_BIT - 1) & ~THREAD_ID_CORE_BIT,
		};

		/**
		* Core process handling class
		* @param threadCount	number of threads used for processes, -1 is #cpu cores
		*/
		Core(s32 threadCount = -1);
		~Core();

		//////////////////////////////////////////////////////////////////////////

		/**
		* Runs the core engine processes.
		* This is a blocking function!
		*/
		void run();

		//! Stops running the core.
		void stop()	{ m_running = false; }


		//////////////////////////////////////////////////////////////////////////

		CoreThread *getThread(u32 threadId)
		{
			assert(threadId < m_threads.size());
			return m_threads[threadId];
		}

		u32 getThreadCount() const	{ return m_threads.size(); }

		//////////////////////////////////////////////////////////////////////////

		/**
		* Gets the next process scheduled to run for a specific thread identifier.
		* @param threadId	The thread to get the next process for.
		* @return a Job pair with job id and Process pointer.
		*/
		CoreJob getNextJob(u32 threadId);

		/**
		* Adds a process to the execution list.
		* @param pProcess	the process to add.
		*/
		void addProcess(Process *pProcess);

		/**
		* Adds a process from the execution list.
		* @param pProcess	the process to remove.
		*/
		void removeProcess(Process *pProcess);

		/**
		 * Runs a single process job, this function is called from CoreThreads as well!
		 */
		void runJob(const CoreJob &job);

		//////////////////////////////////////////////////////////////////////////

		/**
		* Returns the elapsed time since the core system started.
		* @return elapsed time in seconds.
		*/
		double getElapsedTime() const
		{
			os::Time time(os::Time::NOW);
			return (time - m_startTime).getSeconds();
		}

		//! Gets if the core is still running.
		bool isRunning() const							{ return m_running; }

		//! Gets if the core process is currently working.
		const CoreJob &getJob() const				{ return m_job; }

		//! Gets the current resource loader.
		ResourceLoader *getLoader() const				{ return m_pLoader; }
		//! Sets the current resource loader.
		void setLoader(ResourceLoader *pLoader)			{ m_pLoader = pLoader; }

		//! Gets the current render driver.
		render::RenderDriver *getDriver() const			{ return m_pDriver; }
		//! Sets the current render driver.
		void setDriver(render::RenderDriver *pDriver)	{ m_pDriver = pDriver; }

		//! Gets the core log.
		logging::Log * getLog()	{ return m_log; }

	protected:
		os::Time				m_startTime;			// start time of the core system.

		bool					m_running;				// is the core still running.

		ThreadList				m_threads;				// pool of active core threads.

		boost::shared_mutex		m_processesMutex;		// mutex used to lock list of processes.
		boost::shared_mutex		m_waitingProcessesMutex;// mutex used to lock list of processes.

		CoreJob					m_job;					// current job the core process is running.

		ProcessList				m_processes;			// list of processes.
		ProcessList				m_waitingProcesses;		// list of waiting processes.

		u32						m_processRunId;			// incremental identifier to determine process run order.

		ResourceLoader *		m_pLoader;				// current resource loader.
		render::RenderDriver *	m_pDriver;				// current render driver.

		logging::Log *			m_log;					// logger used for core
	};
}

#endif //__CORE_CORE_H__