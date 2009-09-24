#pragma once

#ifndef __FILE_RESOURCE_LOADER_H__
#define __FILE_RESOURCE_LOADER_H__

#include "resources/ResourceLoaderBase.h"

#include "font/FileFontLoader.h"
#include "shader/FileShaderLoader.h"
#include "texture/FileTextureLoader.h"

class FileResourceLoader
	: public ResourceLoaderBase
{
public:
	/**
	 * Constructs a file based resource loader.
	 * This loader basically contains a set of the loaders that
	 * load the actual resources using the File class (or subclasses thereof).
	 * @param types the types of resource loaders to support.
	 */
	FileResourceLoader(ResourceTypeSet &types)
		: m_types(types)
	{
		if (types[RESOURCE_FONT])		addLoader(new FileFontLoader(types));
		if (types[RESOURCE_SHADER])		addLoader(new FileShaderLoader(types));
		if (types[RESOURCE_TEXTURE])	addLoader(new FileTextureLoader(types));
	}

protected:
	ResourceTypeSet	m_types;
};

#endif //__FS_RESOURCE_LOADER_H__
