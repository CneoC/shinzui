#pragma once

#ifndef __RESOUCRES_LOADERS_FONT_FONTCONVERTER_H__
#define __RESOUCRES_LOADERS_FONT_FONTCONVERTER_H__

#include "resources/ResourceLoaderBase.h"
#include "FTFontConverter.h"

class FontConverter
	: public ResourceLoaderBase
{
public:
	/**
	 * Constructs a font converter.
	 * @param types the types of font resource loaders to support.
	 */
	FontConverter(ResourceTypeSet &types)
	{
		if (types[RESOURCE_FT_FONT] && types[RESOURCE_GL_FONT])
			addLoader(new FTFontConverter());
	}

};

#endif //__RESOUCRES_LOADERS_FONT_FONTCONVERTER_H__
