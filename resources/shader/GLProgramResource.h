#pragma once

#ifndef __RESOURCES_GLPROGRAMRESOURCE_H__
#define __RESOURCES_GLPROGRAMRESOURCE_H__

#include "ProgramResource.h"

#include <list>

#include <gl/glew.h>

class GLProgramData
	: public ProgramData
{
public:
	static const char *getName()	{ return "GLProgram"; }

	GLProgramData(ResourceLoaderBase *pLoader)
		: ProgramData(pLoader)
		, m_program(0)
	{
		getType() += getName();
	}

	void setProgram(GLuint program)		{ m_program = program; }
	GLuint getProgram() const			{ return m_program; }

protected:
	GLuint		m_program;
};

typedef ResourceRef<GLProgramData>	GLProgramResource;

#endif //__RESOURCES_GLPROGRAMRESOURCE_H__
