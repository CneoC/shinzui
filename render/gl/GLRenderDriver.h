#pragma once

#ifndef __RENDER_GLRENDERDRIVER_H__
#define __RENDER_GLRENDERDRIVER_H__

#include "render/RenderDriver.h"

#include "os/current/Window.h"
#include "os/current/gl/GLContext.h"

namespace render
{
	class GLRenderDriver
		: public RenderDriver
	{
	public:
		//! 
		GLRenderDriver(core::Core *pCore, os::Window *pWindow);
		~GLRenderDriver();

		//! Get window.
		os::Window *getWindow() const				{ return m_pWindow; }

		//! Get context.
		os::GLContext *getGLContext() const			{ return static_cast<os::GLContext *>(m_pContext); }
		os::GLContext *getLoaderGLContext() const	{ return static_cast<os::GLContext *>(m_pLoaderContext); }

	protected:
		os::Window *	m_pWindow;
	};
}

#endif __RENDER_GLRENDERDRIVER_H__
