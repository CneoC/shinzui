// Only compile on windows builds
#ifdef _WIN32

#include "Window.h"

#include "util/logging/Log.h"

using namespace os;

//////////////////////////////////////////////////////////////////////////

#define WINDOW_CLASS "GameWnd"

Window::Window(core::Core *pCore)
	: WindowBase(pCore)
	, m_hDC(NULL)
	, m_hWnd(NULL)
	, m_hInstance(NULL)
	, m_log(LOG_GET("OS.Win32.Window"))
{
}

Window::~Window()
{
	assert(!m_hDC);
	assert(!m_hWnd);
	assert(!m_hInstance);
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

	m_hInstance			= GetModuleHandle(NULL);

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC)rerouteWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
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
	m_hWnd	= CreateWindowEx(dwExStyle, WINDOW_CLASS, getTitle().c_str(),
							dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
							0, 0,
							rect.right - rect.left,
							rect.bottom - rect.top,
							NULL, NULL, m_hInstance, (LPVOID)this);
	if (!m_hWnd)
	{
		destroy();
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	m_hDC = GetDC(m_hWnd);
	if (!m_hDC)
	{
		destroy();
		MessageBox(NULL,"Can't Create A Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

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

	if (m_hDC && !ReleaseDC(m_hWnd, m_hDC))
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	}
	m_hDC = NULL;

	if (m_hWnd && !DestroyWindow(m_hWnd))
	{
		MessageBox(NULL,"Could Not Release m_hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	}
	m_hWnd = NULL;

	if (!UnregisterClass(WINDOW_CLASS, m_hInstance))
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	}
	m_hInstance = NULL;

	return true;
}

void Window::show()
{
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
}

void Window::hide()
{
	ShowWindow(m_hWnd, SW_HIDE);
}

core::Process *Window::run(u32 job, double delta)
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

	return this;
}

LRESULT CALLBACK Window::wndProc(HWND m_hWnd, UINT uMsg,
								 WPARAM wParam, LPARAM lParam)
{
	LOG_TRACE(m_log, LOG_FMT("window event  msg=%d  wParam=0x%X lParam=0x%X", uMsg % wParam % lParam));

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
			m_pCore->stop();
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
			resize(math::Vector2i(LOWORD(lParam), HIWORD(lParam)));
			return 0;
		}
	}

	// other messages to default window procedure
	return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::rerouteWndProc(HWND m_hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window *pProc = NULL;

	// associate instance from window message data
	if (uMsg == WM_NCCREATE)
	{
		pProc = reinterpret_cast<Window *>(((LPCREATESTRUCT)lParam)->lpCreateParams);
		::SetWindowLong(m_hWnd, GWL_USERDATA, reinterpret_cast<long>(pProc));
	}
	// retrieve previously associated instance
	else
	{
		pProc = reinterpret_cast<Window *>(GetWindowLong(m_hWnd, GWL_USERDATA));
	}

	return pProc->wndProc(m_hWnd, uMsg, wParam, lParam);
}

#endif //_WIN32
