#include "Process.h"

using namespace core;

//////////////////////////////////////////////////////////////////////////

Process::Process(core::Core *pCore, u32 id, u32 threadMask, u32 jobThreadMask)
	: m_pCore(pCore)
	, m_id(id)
	, m_threadMask(threadMask)
	, m_jobThreadMask(jobThreadMask)
	, m_frameDelay(0)
	, m_lastRunTime(0)
	, m_jobs(1)
	, m_activeJobs(0)
	, m_finishedJobs(0)
	, m_color(0.8, 0.9, 1.0)
{
}

bool Process::isDependencyDone() const
{
	if (m_dependencies.empty())
		return true;

	const u32 lastRunId = getLastRunId();
	DependencyProcessList::const_iterator iter = m_dependencies.begin();
	while (iter != m_dependencies.end())
	{
		const u32 otherLastRunId = (*iter)->getLastRunId();
		
		// this process was run after the dependency
		if (lastRunId > otherLastRunId) return false;

		++iter;
	}
	return true;
}
