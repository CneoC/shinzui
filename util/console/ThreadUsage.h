#pragma once

#ifndef __CONSOLE_THREADUSAGE_H__
#define __CONSOLE_THREADUSAGE_H__

#include "render/Renderer.h"

#include "core/Core.h"
#include "os/current/Time.h"

namespace console
{
	class ThreadUsageInfo
		: public os::Thread
	{
	public:
		struct ActivityInfo
		{
			typedef std::list<double>		TimeList;

			ActivityInfo()
				: m_firstActive(false)
				, m_lastActive(false)
			{}

			boost::mutex	m_mutex;
			bool			m_firstActive;
			bool			m_lastActive;
			TimeList		m_times;
		};

		typedef std::vector<ActivityInfo>	ThreadList;

		ThreadUsageInfo(core::Core *pCore, double duration = 1)
			: m_pCore(pCore)
			, m_duration(duration)
		{
			setPriority(PRIORITY_ABOVE_NORMAL);
			m_threads = new ActivityInfo[m_pCore->getThreadCount()];
		}

		virtual bool run()
		{
			double t = m_pCore->getElapsedTime();

			for (u32 i = 0; i < m_pCore->getThreadCount(); i++)
			{
				core::CoreThread *pThread = m_pCore->getThread(i);
				bool active = pThread->isActive();

				ActivityInfo &info = m_threads[i];
				if (info.m_lastActive != active)
				{
					info.m_lastActive = active;
					info.m_times.push_back(t);
				}

				if (t - info.m_times.front() > m_duration)
				{
					boost::lock_guard<boost::mutex> lock(info.m_mutex);
					info.m_times.pop_front();
					info.m_firstActive = !info.m_firstActive;
				}
			}

			return m_pCore->isRunning();
		}

		ActivityInfo &getActivityInfo(u32 i)
		{
			return m_threads[i];
		}

		double getDuration() const	{ return m_duration; }

	protected:
		core::Core *	m_pCore;
		double			m_duration;
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
