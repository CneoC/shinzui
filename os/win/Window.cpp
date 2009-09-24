// Only compile on windows builds
#ifdef _WIN32

#include "Window.h"

#include "util/Log.h"

Window::Window(Core *pCore)
	: WindowBase(pCore)
	, hDC(NULL)
	, hRC(NULL)
	, hWnd(NULL)
{
}

Window::~Window()
{
}

bool Window::create()
{
	WNDCLASS	wc;
	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		rect;

	rect.left	= (long)0;
	rect.right	= (long)getSize().x;
	rect.top	= (long)0;
	rect.bottom	= (long)getSize().y;

	hInstance			= GetModuleHandle(NULL);

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC)rerouteWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "GameWnd";

	// Try to register the window class
	if (!RegisterClass(&wc))
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if (isFullscreen())
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= getSize().x;
		dmScreenSettings.dmPelsHeight	= getSize().y;
		dmScreenSettings.dmBitsPerPel	= getBpp();
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change display settings
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				setFullscreen(false);
			}
			else
			{
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return false;
			}
		}
	}

	// Set window style flags
	if (isFullscreen())
	{
		dwExStyle	= WS_EX_APPWINDOW;
		dwStyle		= WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		dwExStyle	= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle		= WS_OVERLAPPEDWINDOW;
	}

	// Adjust the window rectangle to requested size
	AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);

	// Create the window
	hWnd	= CreateWindowEx(dwExStyle, "GameWnd", getTitle().c_str(),
							dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
							0, 0,
							rect.right - rect.left,
							rect.bottom - rect.top,
							NULL, NULL, hInstance, (LPVOID)this);
	if (!hWnd)
	{
		destroy();
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if (!createContext())
	{
		return false;
	}

	// Show the window
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	return true;
}

bool Window::destroy()
{
	// Reset fullscreen specific settings
	if (isFullscreen())
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	destroyContext();

	if (hDC && !ReleaseDC(hWnd,hDC))
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}

	if (hWnd && !DestroyWindow(hWnd))
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;
	}

	if (!UnregisterClass("GameWnd", hInstance))
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}

	return true;
}

bool Window::resize()
{
	return resizeContext();
}

Process *Window::run(double delta)
{
	MSG msg;

	// Check for windows messages
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return WindowBase::run(delta);
}

LRESULT CALLBACK Window::wndProc(HWND hWnd, UINT uMsg,
										  WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATE:
		{
			if (!HIWORD(wParam))
			{
			}
			else
			{
			}
			return 0;
		}

	case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				return 0;
			}
			break;
		}

	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

	case WM_KEYDOWN:
		{
			//keys[wParam]
			return 0;
		}

	case WM_KEYUP:
		{
			//keys[wParam]
			return 0;
		}

	case WM_SIZE:
		{
			setSize(Vector2i(LOWORD(lParam), HIWORD(lParam)));
			resizeContext();
			return 0;
		}
	}

	// other messages to default window procedure
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::rerouteWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window *pProc = NULL;

	// associate instance from window message data
	if (uMsg == WM_NCCREATE)
	{
		pProc = reinterpret_cast<Window *>(((LPCREATESTRUCT)lParam)->lpCreateParams);
		::SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<long>(pProc));
	}
	// retrieve previously associated instance
	else
	{
		pProc = reinterpret_cast<Window *>(GetWindowLong(hWnd, GWL_USERDATA));
	}

	return pProc->wndProc(hWnd, uMsg, wParam, lParam);
}

#endif //_WIN32
