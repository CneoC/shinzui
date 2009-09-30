#pragma once

#ifndef __RESOURCES_GLPROGRAMRESOURCE_H__
#define __RESOURCES_GLPROGRAMRESOURCE_H__

#include "resources/Resource.h"

#include <list>

#include <gl/glew.h>

class GLProgramData
	: public ResourceData
{
public:
	static const char *getName()	{ return "GLProgram"; }

	GLProgramData(ResourceLoaderBase *pLoader)
		: ResourceData(pLoader)
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

class GLProgramDataDef
	: public ResourceData
{
public:
	typedef std::list<std::string> ShaderList;

	static const char *getName()	{ return "GLProgramDef"; }

	GLProgramDataDef(ResourceLoaderBase *pLoader)
		: ResourceData(pLoader)
	{
		getType() += getName();
	}

	void addShader(const std::string &shader)	{ m_shaders.push_back(shader); }
	void clearShaders()							{ m_shaders.clear(); }

	const ShaderList &getShaders() const		{ return m_shaders; }

protected:
	ShaderList	m_shaders;
};

typedef ResourceRef<GLProgramDataDef>	GLProgramDefinition;

#endif //__RESOURCES_GLPROGRAMRESOURCE_H__
