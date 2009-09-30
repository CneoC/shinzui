#pragma once

#ifndef __RENDER_GLRENDERER_H__
#define __RENDER_GLRENDERER_H__

#include "render/Renderer.h"
#include "render/RenderDriver.h"

#include "os/current/gl/GLContext.h"

namespace render
{
	/**
	* Base class for a renderer.
	*/
	class GLRenderer
		: public Renderer
	{
	protected:
		//! Don't allow external construction.
		GLRenderer(Core *pCore)
			: Renderer(pCore)
		{}

	public:

		//! Get context.
		GLContext *getGLContext() const	{ return static_cast<GLContext *>(m_pCore->getDriver()->getContext()); }
	};
}

#endif //__RENDER_GLRENDERER_H__
