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

		void setFrameBuffer(resources::FrameBufferResource frameBuffer)	{ m_frameBuffer = frameBuffer; }
		resources::FrameBufferResource getFrameBuffer()					{ return m_frameBuffer; }

		void setProgram(resources::ProgramResource program)				{ m_program = program; }
		resources::ProgramResource getProgram()							{ return m_program; }

	protected:
		resources::FrameBufferResource	m_frameBuffer;
		resources::ProgramResource		m_program;
	};
}

#endif //__RENDER_ENDFRAMEBUFFER_H__
