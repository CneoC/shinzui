#pragma once

#ifndef __RESOURCES_GLPROGRAMRESOURCE_H__
#define __RESOURCES_GLPROGRAMRESOURCE_H__

#include "resources/Resource.h"

#include <gl/glew.h>

class GLProgramData
	: public ResourceData
{
public:
	enum { TYPE = RESOURCE_GL_PROGRAM };

	GLProgramData(ResourceLoaderBase *pLoader)
		: ResourceData(pLoader)
		, m_program(0)
	{
		setType(TYPE);
	}

	void setProgram(GLuint program)		{ m_program = program; }
	GLuint getProgram() const			{ return m_program; }

protected:
	GLuint		m_program;
};

typedef ResourceRef<GLProgramData>	GLProgramResource;

class GLProgramDataDef
	: public ResourceData
{
public:
	typedef std::list<std::string> ShaderList;

	enum { TYPE = RESOURCE_GL_PROGRAM_DEFINITION };

	GLProgramDataDef(ResourceLoaderBase *pLoader)
		: ResourceData(pLoader)
	{
		setType(TYPE);
	}

	void addShader(const std::string &shader)	{ m_shaders.push_back(shader); }
	void clearShaders()							{ m_shaders.clear(); }

	const ShaderList &getShaders() const		{ return m_shaders; }

protected:
	ShaderList	m_shaders;
};

typedef ResourceRef<GLProgramDataDef>	GLProgramDefinition;

#endif //__RESOURCES_GLPROGRAMRESOURCE_H__
