#pragma once

#ifndef __RESOURCES_RESOURCECONVERTERS_H__
#define __RESOURCES_RESOURCECONVERTERS_H__

#include "resources/ResourceLoaderBase.h"
#include "resources/converters/font/FontConverter.h"
#include "resources/converters/shader/ShaderConverter.h"
#include "resources/converters/texture/TextureConverter.h"
#include "resources/ResourceType.h"

class ResourceConverters
	: public ResourceLoaderBase
{
public:
	/**
	* Constructs a new file resource loader for raw disk access resources.
	*/
	ResourceConverters()
	{
		addLoader(new FontConverter);
		addLoader(new ShaderConverter);
		addLoader(new TextureConverter);
	}
};

#endif //__RESOURCES_RESOURCECONVERTERS_H__
