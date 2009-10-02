#pragma once

#ifndef __RENDER_ENDFRAMEBUFFER_H__
#define __RENDER_ENDFRAMEBUFFER_H__

#include "render/Renderer.h"

#include "resources/shader/ProgramResource.h"

namespace render
{
	class EndFrameBuffer
		: public Renderer
	{
	public:
		EndFrameBuffer(Core *pCore) : Renderer(pCore) {}

		void setProgram(const ProgramResource &program)	{ m_program = program; }

	protected:
		ProgramResource	m_program;
	};
}

#endif //__RENDER_ENDFRAMEBUFFER_H__
