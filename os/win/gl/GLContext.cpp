// Only compile on windows builds
#ifdef _WIN32

#include "GLContext.h"

#include <gl/glew.h>
#include <gl/wglew.h>

using namespace os;

//////////////////////////////////////////////////////////////////////////

void GLContext::swapBuffers()
{
	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);
	SwapBuffers(m_pWindow->getHDC());
}

bool GLContext::create()
{
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		m_pWindow->getBpp(),
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

	// Choose a pixel format
	GLuint pixelFormat = ChoosePixelFormat(m_pWindow->getHDC(), &pfd);
	if (!pixelFormat)
	{
		destroy();
		throw std::runtime_error("Unable to find proper pixel format.");
		return false;
	}

	// Set the pixel format
	if (!SetPixelFormat(m_pWindow->getHDC(), pixelFormat, &pfd))
	{
		destroy();
		throw std::runtime_error("Unable to set pixel format.");
		return false;
	}

	// Create rendering context
	m_hRC = wglCreateContext(m_pWindow->getHDC());
	if (!m_hRC)
	{
		destroy();
		throw std::runtime_error("Unable to create GL context.");
		return false;
	}

	// Bind to current thread
	if (!bind())
	{
		destroy();
		throw std::runtime_error("Unable to activate GL context.");
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

	if (!resize(m_pWindow->getSize()))
	{
		destroy();
		return false;
	}

	return true;
}

bool GLContext::destroy()
{
	// Release rendering context
	if (m_hRC)
	{
		if (wglGetCurrentContext() && !wglMakeCurrent(NULL, NULL))
		{
			throw std::runtime_error("Unable to release GL context.");
		}

		if (!wglDeleteContext(m_hRC))
		{
			throw std::runtime_error("Unable to destroy GL context.");
		}
		m_hRC = NULL;
	}

	return true;
}

bool GLContext::bind()
{
	return wglMakeCurrent(m_pWindow->getHDC(), m_hRC) == TRUE;
}

bool GLContext::unbind()
{
	return wglMakeCurrent(m_pWindow->getHDC(), NULL) == TRUE;
}

bool GLContext::link(ContextBase *pOther)
{
	return wglShareLists(m_hRC, static_cast<GLContext *>(pOther)->getHRC()) == TRUE;
}

void GLContext::update()
{
	if (m_flags[FLAG_RESIZED])
	{
		m_flags[FLAG_RESIZED] = false;
		resize(m_pWindow->getSize());
	}
}

bool GLContext::resize(const math::Vector2i &size)
{
	LOG_INFO(LOG_GET_ROOT, " resolution " << size.x << "x" << size.y);
	glViewport(0, 0, size.x, size.y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)size.x / size.y, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}

#endif //_WIN32
