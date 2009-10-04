#pragma once

#ifndef __RENDER_GLSTARTRENDERER_H__
#define __RENDER_GLSTARTRENDERER_H__

#include "render/core/StartRenderer.h"

#include "os/common//ContextBase.h"

namespace render
{
	class GLStartRenderer :
		public StartRenderer
	{
	public:
		static Renderer *create(core::Core *pCore)	{ return new GLStartRenderer(pCore); }

		//////////////////////////////////////////////////////////////////////////

		GLStartRenderer(core::Core *pCore);
		~GLStartRenderer();

		//////////////////////////////////////////////////////////////////////////

		virtual void init();
		virtual void render(double delta);

	protected:
	};
}

#endif //__RENDER_GLSTARTRENDERER_H__
