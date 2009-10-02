#pragma once

#ifndef __RENDER_STARTFRAMEBUFFER_H__
#define __RENDER_STARTFRAMEBUFFER_H__

#include "render/Renderer.h"

namespace render
{
	class StartFrameBuffer
		: public Renderer
	{
	public:
		StartFrameBuffer(Core *pCore) : Renderer(pCore) {}
	};
}

#endif //__RENDER_STARTFRAMEBUFFER_H__
