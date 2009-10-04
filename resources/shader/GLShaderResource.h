#pragma once

#ifndef __RESOURCES_GLSHADERRESOURCE_H__
#define __RESOURCES_GLSHADERRESOURCE_H__

#include "ShaderResource.h"

#include <gl/glew.h>

class GLShaderData
	: public ShaderData
{
public:
	enum ShaderType
	{
		TYPE_NONE,
		TYPE_VERTEX,
		TYPE_FRAGMENT
	};

	static const char *getName()		{ return "GLShader"; }
	static const char *getVertName()	{ return "GLVertShader"; }
	static const char *getFragName()	{ return "GLFragShader"; }

	GLShaderData(ResourceLoaderBase *pLoader)
		: ShaderData(pLoader)
		, m_shaderType(TYPE_NONE)
		, m_shader(0)
	{
		getType() += getName();
		m_flags[FLAG_REQUIRE_CONTEXT] = true;
	}

	void setShaderType(ShaderType type)	{ m_shaderType = type; }
	ShaderType getShaderType() const	{ return m_shaderType; }

	void setShader(GLuint shader)		{ m_shader = shader; }
	GLuint getShader() const			{ return m_shader; }

protected:
	ShaderType	m_shaderType;
	GLuint		m_shader;
};

typedef ResourceRef<GLShaderData>	GLShaderResource;

#endif //__RESOURCES_GLSHADERRESOURCE_H__
