#pragma once

#ifndef __RENDER_GLENDFRAMEBUFFER_H__
#define __RENDER_GLENDFRAMEBUFFER_H__

#include "render/Renderer.h"

#include "resources/shader/GLProgramResource.h"

class GLEndFrameBuffer
	: public Renderer
{
public:
	GLEndFrameBuffer(Core *pCore);
	~GLEndFrameBuffer();

	virtual Process *run(double delta);

	void setProgram(const GLProgramResource &program)	{ m_program = program; }

protected:
	GLProgramResource	m_program;
};

#endif //__RENDER_GLENDFRAMEBUFFER_H__
