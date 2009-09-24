#include "Process.h"

Process::Process(Core *pCore, int id, int targetThreadId)
	: m_pCore(pCore)
	, m_id(id)
	, m_targetThreadId(targetThreadId)
	, m_frameDelay(0)
	, m_lastRunTime(0)
{
}

bool Process::isDependencyDone() const
{
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
