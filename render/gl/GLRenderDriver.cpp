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
// GLRenderDriver.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "GLRenderDriver.h"

#include "render/gl/core/GLStartRenderer.h"
#include "render/gl/core/GLEndRenderer.h"
#include "render/gl/core/GLStartFrameBuffer.h"
#include "render/gl/core/GLEndFrameBuffer.h"

#include "render/gl/2d/GL2DLoader.h"

using namespace render;

//////////////////////////////////////////////////////////////////////////

GLRenderDriver::GLRenderDriver(core::Core *pCore, os::Window *pWindow)
	: RenderDriver(pCore)
	, m_pWindow(pWindow)
{
	setPixelFormat();

	m_pContext = new os::GLContext(pWindow);
	getGLContext()->create();
	getGLContext()->bind();
	glewInit();
	wglewInit();
	init();
	getGLContext()->unbind();

	m_pLoaderContext = new os::GLContext(pWindow);
	getGLLoaderContext()->create();
	getGLLoaderContext()->bind();
	glewInit();
	wglewInit();
	getGLLoaderContext()->link(getGLContext());
	getGLLoaderContext()->unbind();

	// Register core renderers
	addRenderer("Start",	&GLStartRenderer::create);
	addRenderer("End",		&GLEndRenderer::create);
	addRenderer("StartFB",	&GLStartFrameBuffer::create);
	addRenderer("EndFB",	&GLEndFrameBuffer::create);

	// Register core utilities

	// Register core loaders
	addLoader("2d", new GL2DLoader(pCore));
}

GLRenderDriver::~GLRenderDriver()
{
	getGLContext()->destroy();
	getGLLoaderContext()->destroy();
}

void GLRenderDriver::setPixelFormat()
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
		throw std::runtime_error("Unable to find proper pixel format.");
	}

	// Set the pixel format
	if (!SetPixelFormat(m_pWindow->getHDC(), pixelFormat, &pfd))
	{
		throw std::runtime_error("Unable to set pixel format.");
	}
}

void GLRenderDriver::init()
{
	// Initialize some standard OpenGL settings
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

	if (!getGLContext()->resize(m_pWindow->getSize()))
	{
		throw std::runtime_error("Unable to set resize context.");
	}
}
