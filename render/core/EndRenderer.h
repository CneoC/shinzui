#pragma once

#ifndef __RENDER_ENDRENDERER_H__
#define __RENDER_ENDRENDERER_H__

#include "render/Renderer.h"

namespace render
{
	class EndRenderer :
		public Renderer
	{
	public:
		EndRenderer(core::Core *pCore) : Renderer(pCore) {}

	protected:
	};
}

#endif //__RENDER_ENDRENDERER_H__
