#pragma once

#ifndef __RENDER_STARTFRAMEBUFFER_H__
#define __RENDER_STARTFRAMEBUFFER_H__

#include "render/Renderer.h"

#include "resources/texture/FrameBufferResource.h"

namespace render
{
	class StartFrameBuffer
		: public Renderer
	{
	public:
		StartFrameBuffer(Core *pCore) : Renderer(pCore) {}

		void setFrameBuffer(FrameBufferResource frameBuffer)	{ m_frameBuffer = frameBuffer; }
		FrameBufferResource getFrameBuffer()					{ return m_frameBuffer; }

	protected:
		FrameBufferResource	m_frameBuffer;
	};
}

#endif //__RENDER_STARTFRAMEBUFFER_H__
