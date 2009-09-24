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

	virtual bool create();
	virtual bool destroy();
	virtual bool resize();

protected:
	virtual bool createContext() = 0;
	virtual bool destroyContext() = 0;
	virtual bool resizeContext() = 0;

	LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK rerouteWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HDC			hDC;		// Private GDI Device Context
	HGLRC		hRC;		// Permanent Rendering Context
	HWND		hWnd;		// Holds Our Window Handle
	HINSTANCE	hInstance;	// Holds The Instance Of The Application
};

#endif //__WINDOW_H__

#endif //_WIN32
