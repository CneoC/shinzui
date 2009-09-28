#pragma once

#ifndef __GL_CONTEXT_H__
#define __GL_CONTEXT_H__

#include "os/interface/ContextBase.h"
#include "os/current/Window.h"

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

	bool create();
	bool destroy();

	bool bind();
	bool unbind();

	void swapBuffers();

	void onResize()			{ resize(m_pWindow->getSize()); }
	
	HGLRC getHRC() const	{ return m_hRC; }

protected:
	bool init();
	bool resize(const Vector2i &size);

protected:
	HGLRC		m_hRC;		// OpenGL rendering context
};

#endif //__GL_CONTEXT_H__
