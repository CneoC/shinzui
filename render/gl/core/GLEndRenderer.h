#pragma once

#ifndef __RENDER_GLENDRENDERER_H__
#define __RENDER_GLENDRENDERER_H__

#include "render/core/EndRenderer.h"

class ContextBase;

namespace render
{
	class GLEndRenderer :
		public EndRenderer
	{
	public:
		static Renderer *create(Core *pCore)	{ return new GLEndRenderer(pCore); }

		//////////////////////////////////////////////////////////////////////////

		GLEndRenderer(Core *pCore);
		~GLEndRenderer();

		virtual void render(double delta);

	protected:
	};
}

#endif //__RENDER_GLENDRENDERER_H__
