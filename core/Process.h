#pragma once

#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "Core.h"

#include <list>

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
	 * @param targetThreadId	target thread id to run this process on (THREAD_ID_NONE for any thread)
	 */
	Process(Core *pCore, int id = 0, int targetThreadId = Core::THREAD_ID_NORMAL);

	/**
	 * Runs the process.
	 * @return if the process is added to the run queue after execution.
	 */
	virtual Process *run(double delta) = 0;

	/**
	 * Check if the process is ready for it's run.
	 */
	bool isReady(double delta)
	{
		bool forceStart = m_forceStart;
		m_forceStart = false;
		return forceStart || (delta > m_frameDelay && isDependencyDone());
	}

	/**
	 * Check if other processes this process depends on have finished their execution.
	 */
	bool isDependencyDone() const;

	//! Registers a dependency for this process.
	void addDependency(Process *pDependency)	{ m_dependencies.push_back(pDependency); }
	//! Removes a dependency from this process.
	void removeDependency(Process *pDependency)	{ m_dependencies.remove(pDependency); }

	//! Get internal process identifier.
	int getId() const					{ return m_id; }

	//! Gets the target thread the Process needs to be run on
	int getTargetThreadId() const		{ return m_targetThreadId; }

	//! Get time of last run.
	double getLastRunId() const			{ return m_lastRunId; }
	//! Set time of last run.
	void setLastRunId(u32 id)			{ m_lastRunId = id; }

	//! Get time of last run.
	double getLastRunTime() const		{ return m_lastRunTime; }
	//! Set time of last run.
	void setLastRunTime(double time)	{ m_lastRunTime = time; }

	//! Gets the frame delay time in seconds.
	double getFrameDelay() const		{ return m_frameDelay; }
	//! Sets the frame delay time in seconds.
	void setFrameDelay(bool frameDelay)	{ m_frameDelay = frameDelay; }

	//! Forces the process ready (ignoring dependencies and frame delay).
	void forceStart()					{ m_forceStart = true; }

protected:
	Core					*m_pCore;

	int						m_id;
	int						m_targetThreadId;

	double					m_frameDelay;

	volatile u32			m_lastRunId;
	volatile double			m_lastRunTime;

	DependencyProcessList	m_dependencies;
	bool					m_forceStart;
	bool					m_cleanup;
};

#endif //__CORE_PROCESS_H__
