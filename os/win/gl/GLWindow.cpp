// Only compile on windows builds
#ifdef _WIN32

#include "GLWindow.h"

#include <gl/glew.h>
#include <gl/wglew.h>

Process *GLWindow::run(double delta)
{
	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);
	SwapBuffers(hDC);
	return Window::run(delta);
}

bool GLWindow::createContext()
{
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		getBpp(),
		0, 0, 0, 0, 0, 0,
		0,											// TODO: alpha buffer support
		0,
		0,											// TODO: accumulation buffer support
		0, 0, 0, 0,									//		 accumulation bits
		16,											// TODO: customizable z-buffer
		0,											// TODO: stencil buffer support
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	hDC = GetDC(hWnd);
	if (!hDC)
	{
		destroy();
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	// Choose a pixel format
	GLuint pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (!pixelFormat)
	{
		destroy();
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	// Set the pixel format
	if (!SetPixelFormat(hDC, pixelFormat, &pfd))
	{
		destroy();
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	// Create rendering context
	hRC = wglCreateContext(hDC);
	if (!hRC)
	{
		destroy();
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	// Activate rendering context
	if (!wglMakeCurrent(hDC, hRC))
	{
		destroy();
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	// Initialize some standard OpenGL settings
	glewInit();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	if (WGL_EXT_swap_control)
	{
		wglSwapIntervalEXT(0);
	}

	if (!resizeContext())
	{
		destroy();
		return false;
	}

	return true;
}

bool GLWindow::destroyContext()
{
	// Release rendering context
	if (hRC)
	{
		if (!wglMakeCurrent(NULL, NULL))
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;
	}

	return true;
}

bool GLWindow::resizeContext()
{
	glViewport(0, 0, getSize().x ,getSize().y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)getSize().x / (GLfloat)getSize().y, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}

#endif //_WIN32
