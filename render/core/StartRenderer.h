#pragma once

#ifndef __RENDER_STARTRENDERER_H__
#define __RENDER_STARTRENDERER_H__

#include "render/Renderer.h"

namespace render
{
	class StartRenderer :
		public Renderer
	{
	public:
		StartRenderer(Core *pCore) : Renderer(pCore) {}
	protected:
	};
}

#endif //__RENDER_STARTRENDERER_H__
