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
// ThreadUsage.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "ThreadUsage.h"

using namespace console;

//////////////////////////////////////////////////////////////////////////

ThreadUsage::ThreadUsage(core::Core *pCore, double keepDuration)
	: m_pCore(pCore)
	, m_keepDuration(keepDuration)
	, m_threadCount(1 + m_pCore->getThreadCount())
{
	m_threads = new ActivityInfo[m_threadCount];
	for (u32 i = 0; i < m_threadCount; i++)
	{
		m_threads[i].m_current.m_pProcess = NULL;
		m_threads[i].m_current.m_start = 0;
		m_threads[i].m_current.m_end = 0;
	}
}

bool ThreadUsage::run()
{
	double t = os::Time(os::Time::NOW).getSeconds();

	for (u32 i = 0; i < m_pCore->getThreadCount(); i++)
	{
		core::Process *pActive;
		if (i == 0)
			pActive = m_pCore->getJob().pProcess;
		else
			pActive = m_pCore->getThread(i - 1)->getJob().pProcess;

		{
			ActivityInfo &info = m_threads[i];
			boost::lock_guard<boost::shared_mutex> lock(info.m_mutex);

			if (pActive != info.m_current.m_pProcess)
			{
				if (info.m_current.m_pProcess)
				{
					info.m_current.m_end = t;
					info.m_list.push_front(info.m_current);
				}

				info.m_current.m_pProcess = pActive;
				info.m_current.m_start	= t;
			}

			while (	!info.m_list.empty() &&
				(t - info.m_list.back().m_end) > m_keepDuration)
			{
				info.m_list.pop_back();
			}
		}
	}

	return m_pCore->isRunning();
}
