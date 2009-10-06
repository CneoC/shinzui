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

		//////////////////////////////////////////////////////////////////////////

		ThreadUsageInfo(core::Core *pCore, double show = 1, double keep = 2);

		virtual bool run();

		//////////////////////////////////////////////////////////////////////////

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

	//////////////////////////////////////////////////////////////////////////

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
