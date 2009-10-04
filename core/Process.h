#pragma once

#ifndef __CORE_PROCESS_H__
#define __CORE_PROCESS_H__

#include "Core.h"

#include "math/Color3.h"

#include <list>

namespace core
{
	/**
	 * Process base class, inherited by core engine processes.
	 */
	class Process
	{
	public:
		typedef std::list<Process *> DependencyProcessList;

		/**
		 * Constructs a core process.
		 * @param pCore			core class.
		 * @param id				process identifier for lookups.
		 * @param threadMask	target thread id to run this process on (THREAD_ID_NONE for any thread)
		 */
		Process(core::Core *pCore, u32 id = 0,
				u32 threadMask = core::Core::THREAD_ID_NORMAL_MASK,
				u32 jobThreadMask = core::Core::THREAD_ID_NORMAL_MASK);

		/**
		 * Initializes the process.
		 * this function is called from a thread in the getThreadMask()
		 *  for thread specific initialization.
		 */
		virtual void init() {}

		/**
		 * Runs the process.
		 * @return if the process is added to the run queue after execution.
		 */
		virtual Process *run(u32 job, double delta) = 0;

		/**
		 * Check if other processes this process depends on have finished their execution.
		 */
		bool isDependencyDone() const;

		//! Registers a dependency for this process.
		void addDependency(Process *pDependency)	{ m_dependencies.push_back(pDependency); }
		//! Removes a dependency from this process.
		void removeDependency(Process *pDependency)	{ m_dependencies.remove(pDependency); }

		//! Get internal process identifier.
		u32 getId() const						{ return m_id; }

		//! Gets the thread the Process needs to be run on.
		u32 getThreadMask() const				{ return m_threadMask; }
		//! Sets the thread the Process needs to be run on.
		void setThreadMask(u32 mask)			{ m_threadMask = mask; }

		//! Gets the thread the jobs for this Process want to run on.
		u32 getJobThreadMask() const			{ return m_jobThreadMask; }
		//! Sets the thread the jobs for this Process want to run on.
		void setJobThreadMask(u32 mask)			{ m_jobThreadMask = mask; }

		//! Gets the amount of jobs this process will allocate for each run.
		u16 getJobs() const						{ return m_jobs; }
		//! Sets the amount of jobs this process will allocate for each run.
		void setJobs(u16 jobs)					{ m_jobs = jobs; }

		//! Gets the amount of active jobs.
		u16 getActiveJobs() const
		{
			boost::shared_lock<boost::shared_mutex> lock(m_jobMutex);
			return m_activeJobs;
		}
		//! Increases the amount of jobs that have activated this run.
		u16 incActiveJobs()
		{
			boost::lock_guard<boost::shared_mutex> lock(m_jobMutex);
			return ++m_activeJobs;
		}
		
		//! Gets the amount of finished jobs.
		u16 getFinishedJobs() const
		{ 
			boost::shared_lock<boost::shared_mutex> lock(m_jobMutex);
			return m_finishedJobs; 
		}
		//! Increases the amount of jobs that have finished this run.
		u16 incFinishedJobs()
		{
			boost::lock_guard<boost::shared_mutex> lock(m_jobMutex);
			return ++m_finishedJobs;
		}

		//! Checks if this job is the core (first) job for this process.
		bool isCoreJob() const	{ return getFinishedJobs() == 0; }
		
		//! Resets the amounts of activated and finished jobs.
		void resetJobs()
		{
			boost::lock_guard<boost::shared_mutex> lock(m_jobMutex);
			m_activeJobs = m_finishedJobs = 0;
		}

		//! Gets the frame delay time in seconds.
		double getFrameDelay() const			{ return m_frameDelay; }
		//! Sets the frame delay time in seconds.
		void setFrameDelay(double frameDelay)	{ m_frameDelay = frameDelay; }

		//! Get id of last run.
		u32 getLastRunId() const				{ return m_lastRunId; }
		//! Set id of last run.
		void setLastRunId(u32 id)				{ m_lastRunId = id; }

		//! Get time of last run.
		double getLastRunTime() const			{ return m_lastRunTime; }
		//! Set time of last run.
		void setLastRunTime(double time)		{ m_lastRunTime = time; }

		//! Gets the next time the process wants to be run.
		double getNextRunTime() const			{ return m_lastRunTime + m_frameDelay; }


		//! Forces the process ready (ignoring dependencies and frame delay).
		void forceStart()						{ m_forceStart = true; }

		//! Gets process identifying color.
		const math::Color3f &getColor() const	{ return m_color; }

	protected:
		core::Core *			m_pCore;

		u32						m_id;
		u32						m_threadMask;
		u32						m_jobThreadMask;

		mutable boost::shared_mutex	m_jobMutex;
		u16						m_jobs;
		u16						m_activeJobs;
		u16						m_finishedJobs;

		double					m_frameDelay;

		volatile u32			m_lastRunId;
		volatile double			m_lastRunTime;

		bool					m_forceStart;
		DependencyProcessList	m_dependencies;

		math::Color3f			m_color;
	};
}

#endif //__CORE_PROCESS_H__
