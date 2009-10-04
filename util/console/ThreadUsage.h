#pragma once

#ifndef __CONSOLE_THREADUSAGE_H__
#define __CONSOLE_THREADUSAGE_H__

#include "render/Renderer.h"

#include "core/Core.h"
#include "core/Process.h"
#include "os/current/Time.h"

namespace console
{
	class ThreadUsageInfo
		: public os::Thread
	{
	public:
		struct Activity
		{
			double			m_start;
			double			m_end;
			core::Process *	m_pProcess;
		};

		struct ActivityInfo
		{
			typedef std::list<Activity>		ActivityList;

			Activity			m_current;
			ActivityList		m_list;
			boost::shared_mutex	m_mutex;
		};

		typedef std::vector<ActivityInfo>	ThreadList;

		ThreadUsageInfo(core::Core *pCore, double show = 0.5, double keep = 3)
			: m_pCore(pCore)
			, m_showDuration(show)
			, m_keepDuration(keep)
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

		virtual bool run()
		{
			double t = os::Time(os::Time::NOW).getSeconds();

			for (u32 i = 0; i < m_threadCount; i++)
			{
				core::Process *pActive;
				if (i == 0)
					pActive = m_pCore->getJob().second;
				else
					pActive = m_pCore->getThread(i - 1)->getJob().second;

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

		ActivityInfo &getActivityInfo(u32 i)
		{
			return m_threads[i];
		}

		double getShowDuration() const	{ return m_showDuration; }
		double getKeepDuration() const	{ return m_keepDuration; }
		u32 getThreadCount() const		{ return m_threadCount; }

	protected:
		core::Core *	m_pCore;
		double			m_showDuration;
		double			m_keepDuration;
		u32				m_threadCount;
		ActivityInfo *	m_threads;
	};

	class ThreadUsage
		: public render::Renderer
	{
	public:
		ThreadUsage(core::Core *pCore);
		~ThreadUsage();

		virtual void render(double delta);

	protected:
		ThreadUsageInfo	m_info;
	};
}

#endif //__CONSOLE_THREADUSAGE_H__
