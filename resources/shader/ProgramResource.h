#pragma once

#ifndef __RESOURCES_PROGRAMRESOURCE_H__
#define __RESOURCES_PROGRAMRESOURCE_H__

#include "resources/Resource.h"

#include <list>

#include <gl/glew.h>

class ProgramData
	: public ResourceData
{
public:
	typedef std::list<std::string> ShaderList;

	static const char *getName()	{ return "Program"; }

	ProgramData(ResourceLoaderBase *pLoader)
		: ResourceData(pLoader)
	{
		getType() += getName();
	}

	virtual void bind() {}
	virtual void unbind() {}

	void addShader(const std::string &shader)	{ m_shaders.push_back(shader); }
	void clearShaders()							{ m_shaders.clear(); }

	const ShaderList &getShaders() const		{ return m_shaders; }

protected:
	ShaderList	m_shaders;
};

typedef ResourceRef<ProgramData>	ProgramResource;

#endif //__RESOURCES_PROGRAMRESOURCE_H__
