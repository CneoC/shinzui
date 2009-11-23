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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shinzui. If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////
//
// GLContext.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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
	// Create rendering context
	m_hRC = wglCreateContext(m_pWindow->getHDC());
	if (!m_hRC)
	{
		destroy();
		throw std::runtime_error("Unable to create GL context.");
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
	bool status = wglMakeCurrent(m_pWindow->getHDC(), m_hRC) == TRUE;
	if (!status)
	{
		throw std::runtime_error("Unable to bind GL context.");
		return false;
	}
	return ContextBase::bind();
}

bool GLContext::unbind()
{
	bool status = wglMakeCurrent(NULL, NULL) == TRUE;
	if (!status)
	{
		throw std::runtime_error("Unable to ubbind GL context.");
		return false;
	}
	return ContextBase::unbind();
}

bool GLContext::link(ContextBase *pOther)
{
	bool status = wglShareLists(m_hRC, static_cast< GLContext * >(pOther)->getHRC()) == TRUE;
	if (!status)
	{
		throw std::runtime_error("Unable to link GL contexts.");
	}
	return status;
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
