#pragma once

#ifndef __RENDER_GLRENDERDRIVER_H__
#define __RENDER_GLRENDERDRIVER_H__

#include "render/RenderDriver.h"

namespace render
{
	class GLRenderDriver
		: public RenderDriver
	{
	public:
		//! 
		GLRenderDriver(Core *pCore, Window *pWindow)
			: RenderDriver(pCore)
			, m_pWindow(pWindow)
		{
			m_pContext = new GLContext(pWindow);
			getGLContext()->create();
		}

		~GLRenderDriver()
		{
			getGLContext()->destroy();
		}


		//! Get window.
		Window *getWindow() const	{ return m_pWindow; }

		//! Get context.
		GLContext *getGLContext() const	{ return static_cast<GLContext *>(m_pContext); }

	protected:
		Window *	m_pWindow;
	};
}

#endif __RENDER_GLRENDERDRIVER_H__
