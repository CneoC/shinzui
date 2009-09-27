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
	Process(Core *pCore, int id = 0, int targetThreadMask = Core::THREAD_ID_NORMAL_MASK);

	/**
	 * Initializes the process.
	 * this function is called from a thread in the getTargetThreadMask()
	 *  for thread specific initialization.
	 */
	virtual void init() {}

	/**
	 * Runs the process.
	 * @return if the process is added to the run queue after execution.
	 */
	virtual Process *run(double delta) = 0;

	/**
	 * Check if other processes this process depends on have finished their execution.
	 */
	bool isDependencyDone() const;

	//! Registers a dependency for this process.
	void addDependency(Process *pDependency)	{ m_dependencies.push_back(pDependency); }
	//! Removes a dependency from this process.
	void removeDependency(Process *pDependency)	{ m_dependencies.remove(pDependency); }

	//! Get internal process identifier.
	u32 getId() const					{ return m_id; }

	//! Gets the target thread the Process needs to be run on
	u32 getTargetThreadMask() const		{ return m_targetThreadMask; }
	//! Sets the target thread the Process needs to be run on
	void setTargetThreadMask(u32 mask)	{ m_targetThreadMask = mask; }

	//! Get time of last run.
	double getLastRunId() const			{ return m_lastRunId; }
	//! Set time of last run.
	void setLastRunId(u32 id)			{ m_lastRunId = id; }

	//! Get time of last run.
	double getLastRunTime() const		{ return m_lastRunTime; }
	//! Set time of last run.
	void setLastRunTime(double time)	{ m_lastRunTime = time; }

	//! Gets the next time the process wants to be run.
	double getNextRunTime() const		{ return m_lastRunTime + m_frameDelay; }

	//! Gets the frame delay time in seconds.
	double getFrameDelay() const			{ return m_frameDelay; }
	//! Sets the frame delay time in seconds.
	void setFrameDelay(double frameDelay)	{ m_frameDelay = frameDelay; }

	//! Forces the process ready (ignoring dependencies and frame delay).
	void forceStart()					{ m_forceStart = true; }

protected:
	Core					*m_pCore;

	int						m_id;
	int						m_targetThreadMask;

	double					m_frameDelay;

	volatile u32			m_lastRunId;
	volatile double			m_lastRunTime;

	DependencyProcessList	m_dependencies;
	bool					m_forceStart;
	bool					m_cleanup;
};

#endif //__CORE_PROCESS_H__
