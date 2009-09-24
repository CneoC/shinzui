#pragma once

#ifndef __SHADER_RESOURCE_H__
#define __SHADER_RESOURCE_H__

#include "resources/Resource.h"

class ShaderData
	: public ResourceData
{
public:
	enum { TYPE = RESOURCE_SHADER };

	ShaderData()
	{
		setType(TYPE);
	}

protected:
};

class GLShaderData
	: public ShaderData
{
public:

};

typedef ResourceRef<ShaderData>		ShaderResource;

#endif //__SHADER_RESOURCE_H__
