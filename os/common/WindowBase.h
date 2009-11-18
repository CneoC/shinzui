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
// WindowBase.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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

		//! Updates the window from the core thread.
		virtual void update() = 0;
		
		// 	virtual bool create() = 0;
		// 	virtual bool destroy() = 0;

		// 	virtual void show() = 0;
		// 	virtual void hide() = 0;

		//! Gets the window title.
		const std::string &getTitle() const			{ return m_title; }
		//! Sets the window title.
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

