#pragma once

// Only compile on windows builds
#ifdef _WIN32

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "os/interface/WindowBase.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class Window :
	public WindowBase
{
public:
	Window(Core *pCore);
	~Window();
	
	virtual Process *run(double delta);

	bool create();
	bool destroy();
	
	void show();
	void hide();

	HDC getHDC() const				{ return m_hDC; }
	HWND getHWnd() const			{ return m_hWnd; }
	HINSTANCE getHInstance() const	{ return m_hInstance; }

protected:
	LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK rerouteWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool resize(const math::Vector2i &size)
	{
		// resizeCallback(size);
		return true;
	}

	HDC				m_hDC;			// GDI device context.
	HWND			m_hWnd;			// window handle.
	HINSTANCE		m_hInstance;	// instance of the program.
	logging::Log *	m_log;
};

#endif //__WINDOW_H__

#endif //_WIN32
