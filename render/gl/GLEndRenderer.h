#pragma once

#ifndef __RENDER_GLENDRENDERER_H__
#define __RENDER_GLENDRENDERER_H__

#include "GLRenderer.h"

class ContextBase;

namespace render
{
	class GLEndRenderer :
		public GLRenderer
	{
	public:
		GLEndRenderer(Core *pCore);
		~GLEndRenderer();

		virtual void render(double delta);

	protected:
	};
}

#endif //__RENDER_GLENDRENDERER_H__
