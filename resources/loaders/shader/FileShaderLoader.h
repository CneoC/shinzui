#pragma once

#ifndef __FILE_SHADER_LOADER_H__
#define __FILE_SHADER_LOADER_H__

#include "resources/ResourceLoaderBase.h"

#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

class FileShaderLoader
	: public ResourceLoaderBase
{
public:
	/**
	 * Constructs a file based shader resource loader.
	 * @param types the types of shader resource loaders to support.
	 */
	FileShaderLoader(ResourceTypeSet &types)
	{
		//if (types[RESOURCE_FONT]) addLoader(new FileFontLoader(types));
	}

};

#endif //__FS_SHADER_LOADER_H__
