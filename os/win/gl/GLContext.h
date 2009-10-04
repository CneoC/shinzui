#pragma once

#ifndef __OS_GLCONTEXT_H__
#define __OS_GLCONTEXT_H__

#include "os/common//ContextBase.h"
#include "os/current/Window.h"

namespace os
{
	class GLContext :
		public ContextBase
	{
	public:
		GLContext(Window *pWindow)
			: ContextBase(pWindow)
			, m_hRC(NULL)
		{
		}
		
		~GLContext()
		{
			assert(!m_hRC);
		}

		virtual bool create();
		virtual bool destroy();

		virtual bool bind();
		virtual bool unbind();
		virtual bool link(ContextBase *pOther);

		virtual void update();

		virtual void swapBuffers();
		
		//////////////////////////////////////////////////////////////////////////

		HGLRC getHRC() const	{ return m_hRC; }

	protected:
		bool init();
		bool resize(const math::Vector2i &size);

	protected:
		HGLRC		m_hRC;		// OpenGL rendering context
	};
}

#endif //__OS_GLCONTEXT_H__
