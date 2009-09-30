#pragma once

#ifndef __RENDER_GLSTARTRENDERER_H__
#define __RENDER_GLSTARTRENDERER_H__

#include "GLRenderer.h"

#include "os/interface/ContextBase.h"

namespace render
{
	class GLStartRenderer :
		public GLRenderer
	{
	public:
		GLStartRenderer(Core *pCore);
		~GLStartRenderer();

		virtual void render(double delta);

	protected:
	};
}

#endif //__RENDER_GLSTARTRENDERER_H__
