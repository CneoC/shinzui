#pragma once

#ifndef __RENDER_ENDFRAMEBUFFER_H__
#define __RENDER_ENDFRAMEBUFFER_H__

#include "render/Renderer.h"

#include "resources/texture/FrameBufferResource.h"
#include "resources/shader/ProgramResource.h"

namespace render
{
	class EndFrameBuffer
		: public Renderer
	{
	public:
		EndFrameBuffer(core::Core *pCore) : Renderer(pCore) {}

		void setFrameBuffer(FrameBufferResource frameBuffer)	{ m_frameBuffer = frameBuffer; }
		FrameBufferResource getFrameBuffer()					{ return m_frameBuffer; }

		void setProgram(ProgramResource program)				{ m_program = program; }
		ProgramResource getProgram()							{ return m_program; }

	protected:
		FrameBufferResource	m_frameBuffer;
		ProgramResource		m_program;
	};
}

#endif //__RENDER_ENDFRAMEBUFFER_H__
