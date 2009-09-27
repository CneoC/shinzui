#pragma once

#ifndef __RESOUCRES_LOADERS_FONT_FILEFONTLOADER_H__
#define __RESOUCRES_LOADERS_FONT_FILEFONTLOADER_H__

#include "resources/ResourceLoaderBase.h"
#include "FileFTFontLoader.h"

class FileFontLoader
	: public ResourceLoaderBase
{
public:
	/**
	 * Constructs a file based font resource loader.
	 * @param types the types of font resource loaders to support.
	 */
	FileFontLoader(ResourceTypeSet &types)
	{
		if (types[RESOURCE_FT_FONT])	addLoader(new FileFTFontLoader());
	}

};

#endif //__RESOUCRES_LOADERS_FONT_FILEFONTLOADER_H__
