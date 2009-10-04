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
		static Renderer *create(core::Core *pCore)	{ return new GLEndRenderer(pCore); }

		//////////////////////////////////////////////////////////////////////////

		GLEndRenderer(core::Core *pCore);
		~GLEndRenderer();

		virtual void render(double delta);

	protected:
	};
}

#endif //__RENDER_GLENDRENDERER_H__
