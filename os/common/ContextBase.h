#pragma once

#ifndef __OS_CONTEXTBASE_H__
#define __OS_CONTEXTBASE_H__

#include "os/current/Window.h"

namespace os
{
	class ContextBase
	{
	public:
		ContextBase(Window *pWindow)
			: m_pWindow(pWindow) 
		{
		}

		virtual bool create() = 0;
		virtual bool destroy() = 0;

		virtual bool bind() = 0;
		virtual bool unbind() = 0;
		virtual bool link(ContextBase *pOther) = 0;

		virtual void swapBuffers() = 0;

		//	virtual bool onResize(const Vector2i &size) = 0;

	protected:
		Window*	m_pWindow;
	};
}

#endif //__OS_CONTEXTBASE_H__
