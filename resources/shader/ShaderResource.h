#pragma once

#ifndef __RESOURCES_SHADERRESOURCE_H__
#define __RESOURCES_SHADERRESOURCE_H__

#include "resources/Resource.h"

class ShaderData
	: public ResourceData
{
public:
	enum { TYPE = RESOURCE_SHADER };

	ShaderData(ResourceLoaderBase *pLoader)
		: ResourceData(pLoader)
	{
		setType(TYPE);
	}

protected:
};

typedef ResourceRef<ShaderData>		ShaderResource;

#endif //__RESOURCES_SHADERRESOURCE_H__
