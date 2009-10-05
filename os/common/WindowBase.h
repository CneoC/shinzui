#pragma once

#ifndef __OS_WINDOWBASE_H__
#define __OS_WINDOWBASE_H__

#include "core/Process.h"

#include "math/Vector2.h"
#include "util/Delegate.h"

#include <bitset>

namespace os
{
	class WindowBase :
		public core::Process
	{
	public:
		enum Flags
		{
			FLAG_DIRTY_SIZE,
			FLAG_DIRTY_FULLSCREEN,
			FLAG_DIRTY_TITLE,
			FLAG_DIRTY_BPP,

			NUM_FLAGS
		};

	public:
		WindowBase(core::Core *pCore);
		virtual ~WindowBase();

		//////////////////////////////////////////////////////////////////////////

		virtual void onStart()
		{
			m_pCore->addJob(this, core::Job::Function(this, &WindowBase::updateJob), core::Core::THREAD_ID_CORE_BIT);
		}

		bool updateJob()
		{
			update();
			return false;
		}

		//////////////////////////////////////////////////////////////////////////

		virtual void update() = 0;
		
		// 	virtual bool create() = 0;
		// 	virtual bool destroy() = 0;

		// 	virtual void show() = 0;
		// 	virtual void hide() = 0;

		const std::string &getTitle() const			{ return m_title; }
		void setTitle(const std::string &title)		{ m_title = title; m_flags[FLAG_DIRTY_TITLE] = true; }

		bool isFullscreen() const					{ return m_fullscreen; }
		void setFullscreen(bool fullsreen)			{ m_fullscreen = fullsreen; m_flags[FLAG_DIRTY_FULLSCREEN] = true; }

		const math::Vector2i &getSize() const		{ return m_size; }
		void setSize(const math::Vector2i &size)	{ m_size = size; m_flags[FLAG_DIRTY_SIZE] = true; }

		const int getBpp() const					{ return m_bpp; }
		void setBpp(int bpp)						{ m_bpp = bpp; m_flags[FLAG_DIRTY_BPP] = true; }

		//////////////////////////////////////////////////////////////////////////

		func::delegate< void (const math::Vector2i &) >	resizeEvent;

	protected:
		std::bitset<32>	m_flags;
		bool			m_fullscreen;
		std::string		m_title;
		math::Vector2i	m_size;
		int				m_bpp;
	};
}

#endif //__OS_WINDOWBASE_H__

