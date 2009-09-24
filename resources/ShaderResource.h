#pragma once

#ifndef __SHADER_RESOURCE_H__
#define __SHADER_RESOURCE_H__

#include "resources/Resource.h"

class ShaderData
	: public ResourceData
{
public:
	enum { TYPE = RESOURCE_SHADER };

	ShaderData(u32 type = 0)
		: ResourceData(type | TYPE)
	{
	}

protected:
};

typedef ResourceRef<ShaderData>		ShaderResource;

#endif //__SHADER_RESOURCE_H__
