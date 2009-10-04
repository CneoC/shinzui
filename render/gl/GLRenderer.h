#pragma once

#ifndef __RENDER_GLRENDERER_H__
#define __RENDER_GLRENDERER_H__

#include "render/Renderer.h"
#include "render/RenderDriver.h"

#include "os/current/gl/GLContext.h"

namespace render
{
	/**
	 * Base class for a OpenGL based renderer renderer.
	 */
	class GLRenderer
		: public Renderer
	{
	protected:
		//! Don't allow external construction.
		GLRenderer(core::Core *pCore)
			: Renderer(pCore)
		{}
	};
}

#endif //__RENDER_GLRENDERER_H__
