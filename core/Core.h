#pragma once

#ifndef __CORE_CORE_H__
#define __CORE_CORE_H__

#include <cassert>

#include "types.h"

#include "os/current/Thread.h"
#include "os/current/Time.h"
#include "util/logging/LogManager.h"
#include "util/FastDelegate.h"

namespace resources
{
	class ResourceLoader;
}
namespace render
{
	class RenderDriver;
}

namespace core
{
	class Process;
	class Core;

	/**
	 * Single job definition for a thread.
	 */
	struct Job
	{
		/**
		 * Function pointer to runs the job.
		 * @return if the job should continue running
		 */
		typedef fastdelegate::FastDelegate< bool () > Function;

		Job()
			: threadMask(0)
			, pProcess(NULL)
		{}

		volatile u32	threadMask;	// thread mask for this job, 0 if not active.
		Process *		pProcess;	// process owning the job.
		Function		func;		// function call that runs the job.
	};

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
		{
		}

		//! Runs the core thread
		virtual bool run();

		Core *getCore() const						{ return m_pCore; }

		//! Gets the thread identifier.
		u32 getId() const							{ return m_id; }

		/**
		 * Sets the job to run.
		 * The thread might need to be notified if it's currently waiting for a job.
		 */
		void setJob(const Job &job)					{ m_job = job; }
		//! Gets the currently active job.
		const Job &getJob() const					{ return m_job; }

		//! Gets if the thread is currently working on a job.
		bool isWorking() const						{ return m_job.threadMask != 0; }

		u32 getJobsDone() const						{ return m_jobsDone; }

		//////////////////////////////////////////////////////////////////////////

		// For core thread job wait handling.
		boost::mutex &getProcessMutex()				{ return m_mutex; }
		boost::condition_variable &getCondition()	{ return m_condition; }

	protected:
		Core *	m_pCore;		// parent core class.
		u32		m_id;			// thread identifier bit.

		u32		m_jobsDone;		// number of jobs finished thusfar.
		Job		m_job;			// current job assigned to this thread.

		boost::condition_variable	m_condition;	// condition variable to wait on when no job is readily available.
		boost::mutex				m_mutex;		// mutex to manage waiting.
	};

	/**
	 * Core process handling class.
	 * Handles a number of core engine processes possibly using threading.
	 */
	class Core
	{
	public:
		typedef std::list<Process *>		ProcessList;
		typedef std::list<Job>				JobList;
		typedef std::vector<CoreThread *>	ThreadList;

		/**
		 * Thread type identifiers.
		 */
		enum ThreadID
		{
			THREAD_ID_MAIN,			// Main program thread for Window and Rendering related tasks.
			THREAD_ID_LOAD,			// Special loading thread to load Context requiring resources.
			THREAD_ID_NORMAL_START,	// First id of rest of (normal) threads.

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
		 * This function blocks until some source calls stop() on this class.
		 */
		void run();

		//! Stops running the core.
		void stop()	{ m_running = false; }


		//////////////////////////////////////////////////////////////////////////

		//! Gets a thread with a specific id.
		CoreThread *getThread(u32 threadId)
		{
			assert(threadId < m_threads.size());
			return m_threads[threadId];
		}

		//! Gets the amount of available threads.
		u32 getThreadCount() const	{ return m_threads.size(); }

		// TODO: void setThreadCount(bool block) - dynamically create or stop threads.

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Gets the next process scheduled to run for a specific thread identifier.
		 * @param threadId	The thread to get the next job for.
		 * @param nextJob	The job that was found.
		 * @return If a job was found or not.
		 */
		bool getNextJob(u32 threadId, Job &nextJob);

		/**
		 * Adds a process to the execution list.
		 * @param pProcess	The process to add.
		 */
		void addProcess(Process *pProcess);

		/**
		 * Adds a process from the execution list.
		 * @param pProcess	The process to remove.
		 */
		void removeProcess(Process *pProcess);

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Adds a new process job to the core.
		 * @param pProcess		The process the job belongs to.
		 * @param func			The job execution function pointer.
		 * @param threadMask	The thread the job wants to be run in.
		 */
		void addJob(Process *pProcess, const Job::Function &func, u32 threadMask = THREAD_ID_NORMAL_MASK);

		/**
		 * Utility function to run a single job.
		 * This function is used in CoreThreads as well!
		 */
		void runJob(const Job &job);

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Returns the elapsed time since the core system started.
		 * @return Elapsed time in seconds.
		 */
		double getElapsedTime() const
		{
			os::Time time(os::Time::NOW);
			return (time - m_startTime).getSeconds();
		}

		//! Gets if the core is still running.
		bool isRunning() const								{ return m_running; }

		//! Gets the job the core process is currently working on.
		const Job &getJob() const							{ return m_job; }
		//! Gets if the core process is currently working on a job.
		bool isWorking() const								{ return m_job.threadMask != 0; }

		//! Gets the current resource loader.
		resources::ResourceLoader *getLoader() const		{ return m_pLoader; }
		//! Sets the current resource loader.
		void setLoader(resources::ResourceLoader *pLoader)	{ m_pLoader = pLoader; }

		//! Gets the current render driver.
		render::RenderDriver *getDriver() const				{ return m_pDriver; }
		//! Sets the current render driver.
		void setDriver(render::RenderDriver *pDriver)		{ m_pDriver = pDriver; }

		//! Gets the core log.
		logging::Log * getLog()								{ return m_log; }

	protected:
		os::Time				m_startTime;			// start time of the core system.

		bool					m_running;				// is the core still running.

		//////////////////////////////////////////////////////////////////////////

		ThreadList				m_threads;				// pool of active core threads.

		boost::shared_mutex		m_processesMutex;		// mutex used to lock the list of processes.
		u32						m_processRunId;			// incremental identifier to determine process run order.
		ProcessList				m_processes;			// list of processes.

		boost::shared_mutex		m_jobsMutex;			// mutex used to lock the list of jobs.
		JobList					m_jobs;					// list of queued jobs.
		Job						m_job;					// current job the core process is running internally.

		//////////////////////////////////////////////////////////////////////////

		resources::ResourceLoader *m_pLoader;			// current resource loader.
		render::RenderDriver *	m_pDriver;				// current render driver.

		logging::Log *			m_log;					// logger used for core
	};
}

#endif //__CORE_CORE_H__