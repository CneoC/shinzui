#pragma once

#ifndef __RENDER_GLENDFRAMEBUFFER_H__
#define __RENDER_GLENDFRAMEBUFFER_H__

#include "GLRenderer.h"

#include "resources/shader/GLProgramResource.h"

namespace render
{
	class GLEndFrameBuffer
		: public GLRenderer
	{
	public:
		GLEndFrameBuffer(Core *pCore);
		~GLEndFrameBuffer();

		virtual void render(double delta);

		void setProgram(const GLProgramResource &program)	{ m_program = program; }

	protected:
		GLProgramResource	m_program;
	};
}

#endif //__RENDER_GLENDFRAMEBUFFER_H__
