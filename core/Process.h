//////////////////////////////////////////////////////////////////////////
//
// This file is part of Shinzui.
//
// Shinzui is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shinzui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shinzui.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////
//
// Process.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __CORE_PROCESS_H__
#define __CORE_PROCESS_H__

#include "Core.h"

#include "math/Color3.h"
#include "os/current/AtomicCounter.h"

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
		 */
		Process(core::Core *pCore, u32 id = 0);

		/**
		 * Initializes the process.
		 * this function is called from a thread in the getThreadMask()
		 *  for thread specific initialization.
		 */
		virtual void init() {}

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Called when the process is activated.
		 * Called from main thread, expects process to add one or more jobs to execute.
		 */
		virtual void onStart() = 0;

		/**
		 * Called when all jobs are finished.
		 */
		virtual void onStop() {}

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Check if other processes this process depends on have finished their execution.
		 */
		bool isDependencyDone() const;

		//! Registers a dependency for this process.
		void addDependency(Process *pDependency)	{ m_dependencies.push_back(pDependency); }
		//! Removes a dependency from this process.
		void removeDependency(Process *pDependency)	{ m_dependencies.remove(pDependency); }

		//////////////////////////////////////////////////////////////////////////

		//! Gets the amount of active jobs.
		u32 getJobs() const		{ return m_jobs; }
		//! Increases the amount of jobs that are active for this process.
		void incJobs()			{ ++m_jobs; }
		//! Decreases the amount of jobs that are active for this process.
		void decJobs()			{ --m_jobs; }
		//! Resets the amounts of activated jobs.
		void resetJobs()		{ m_jobs = 0; }

		//////////////////////////////////////////////////////////////////////////

		//! Get internal process identifier.
		u32 getId() const						{ return m_id; }

		//! Gets the core class this process belongs to.
		core::Core *getCore() const				{ return m_pCore; }

		//! Gets the frame delay time in seconds.
		float getDelay() const					{ return m_delay; }
		//! Sets the frame delay time in seconds.
		void setDelay(float delay)				{ m_delay = delay; }

		//! Get id of last run, used for dependency checking.
		u32 getLastRunId() const				{ return m_lastRunId; }
		//! Set id of last run.
		void setLastRunId(u32 id)				{ m_lastRunId = id; }

		//! Get time of last run.
		double getLastRunTime() const			{ return m_lastRunTime; }
		//! Set time of last run.
		void setLastRunTime(double time)		{ m_lastRunTime = time; }

		//! Get delta time of current run.
		float getDeltaTime() const				{ return m_deltaTime; }
		//! Set delta time of current run.
		void setDeltaTime(float time)			{ m_deltaTime = time; }

		//! Gets the next time the process wants to be run.
		double getNextRunTime() const			{ return m_lastRunTime + m_delay; }

		//! Forces the process ready (ignoring dependencies and frame delay).
		void forceStart()						{ m_forceStart = true; }

		//! Gets process identifying color.
		const math::Color3f &getColor() const	{ return m_color; }

	protected:
		core::Core *			m_pCore;			// the core this process belongs to.

		u32						m_id;				// identifier of this process.

		os::AtomicCounter<u32>	m_jobs;				// amount of active jobs this process has.
		
		float					m_delay;			// wait time between activations for this process.

		u32						m_lastRunId;		// last run identifier (used for dependency checking).
		double					m_lastRunTime;		// last time the process was run.
		float					m_deltaTime;		// time since last run from when the process was activated.

		bool					m_forceStart;		// force the process to start regardless of checks.
		DependencyProcessList	m_dependencies;		// list of processes this process dependends on before it can start.

		math::Color3f			m_color;			// process color used by util::ThreadUsage
	};
}

#endif //__CORE_PROCESS_H__
