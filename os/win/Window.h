#pragma once

// Only compile on windows builds
#ifdef _WIN32

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "core/WindowBase.h"

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

	bool resize(const Vector2i &size)
	{
		// resizeCallback(size);
		return true;
	}

	HDC			m_hDC;			// Private GDI Device Context
	HWND		m_hWnd;			// Holds Our Window Handle
	HINSTANCE	m_hInstance;	// Holds The Instance Of The Application
};

#endif //__WINDOW_H__

#endif //_WIN32
