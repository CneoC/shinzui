#pragma once

#ifndef __CONTEXT_BASE_H__
#define __CONTEXT_BASE_H__

#include "os/current/Window.h"

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

	virtual void swapBuffers() = 0;

//	virtual bool onResize(const Vector2i &size) = 0;

protected:
	Window*	m_pWindow;
};

#endif //__CONTEXT_BASE_H__
