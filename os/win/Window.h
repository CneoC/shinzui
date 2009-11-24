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
// Window.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

// Only compile on windows builds
#ifdef _WIN32

#ifndef __OS_WINDOW_H__
#define __OS_WINDOW_H__

#include "os/common//WindowBase.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//////////////////////////////////////////////////////////////////////////

namespace os
{
	/**
	 * Windows based Window class.
	 */
	class Window :
		public WindowBase
	{
	public:
		Window(core::Core *pCore);
		~Window();

		//////////////////////////////////////////////////////////////////////////

		virtual void update();

		bool create();
		bool destroy();

		void show();
		void hide();

		//////////////////////////////////////////////////////////////////////////

		HDC getHDC() const				{ return m_hDC; }
		HWND getHWnd() const			{ return m_hWnd; }
		HINSTANCE getHInstance() const	{ return m_hInstance; }

	protected:
		LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK rerouteWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool resize(const math::Vector2i &size)
		{
			m_size = size;
			resizeEvent(size);
			return true;
		}

		//////////////////////////////////////////////////////////////////////////

		HDC				m_hDC;			// GDI device context.
		HWND			m_hWnd;			// window handle.
		HINSTANCE		m_hInstance;	// instance of the program.
		logging::Log *	m_log;
	};
}

#endif //__OS_WINDOW_H__

#endif //_WIN32
