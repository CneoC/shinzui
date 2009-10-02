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
		GLRenderDriver(Core *pCore, Window *pWindow);
		~GLRenderDriver();

		//! Get window.
		Window *getWindow() const	{ return m_pWindow; }

		//! Get context.
		GLContext *getGLContext() const			{ return static_cast<GLContext *>(m_pContext); }
		GLContext *getLoaderGLContext() const	{ return static_cast<GLContext *>(m_pLoaderContext); }

	protected:
		Window *	m_pWindow;
	};
}

#endif __RENDER_GLRENDERDRIVER_H__
