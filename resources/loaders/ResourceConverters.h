#pragma once

#ifndef __RESOURCES_RESOURCECONVERTERS_H__
#define __RESOURCES_RESOURCECONVERTERS_H__

#include "resources/ResourceLoaderBase.h"
#include "resources/loaders/font/FontConverter.h"
#include "resources/loaders/shader/ShaderConverter.h"
#include "resources/loaders/texture/TextureConverter.h"
#include "resources/ResourceTypes.h"

class ResourceConverters
	: public ResourceLoaderBase
{
public:
	/**
	* Constructs a new file resource loader for raw disk access resources.
	*/
	ResourceConverters(ResourceTypeSet types)
		: m_types(types)
	{
		if (types[RESOURCE_FONT])		addLoader(new FontConverter(types));
		if (types[RESOURCE_SHADER])		addLoader(new ShaderConverter(types));
		if (types[RESOURCE_TEXTURE])	addLoader(new TextureConverter(types));
	}

protected:
	ResourceTypeSet	m_types;
};

#endif //__RESOURCES_RESOURCECONVERTERS_H__
