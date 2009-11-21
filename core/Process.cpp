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
// Process.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "Process.h"

using namespace core;

//////////////////////////////////////////////////////////////////////////

Process::Process(core::Core *pCore, u32 id)
	: m_pCore(pCore)
	, m_id(id)
	, m_delay(0)
	, m_lastRunTime(0)
	, m_jobs(0)
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
