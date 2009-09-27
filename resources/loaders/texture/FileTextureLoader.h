#pragma once

#ifndef __FILE_TEXTURE_LOADER_H__
#define __FILE_TEXTURE_LOADER_H__

#include "resources/ResourceLoaderBase.h"

class FileTextureLoader
	: public ResourceLoaderBase
{
public:
	/**
	 * Constructs a file based texture resource loader.
	 * @param types the types of texture resource loaders to support.
	 */
	FileTextureLoader(ResourceTypeSet &types)
	{
		//if (types[RESOURCE_FONT]) addLoader(new FileFontLoader(types));
	}
};

#endif //__FS_TEXTURE_LOADER_H__
