#pragma once

#ifndef __RESOURCES_FONTCONVERTER_H__
#define __RESOURCES_FONTCONVERTER_H__

#include "resources/ResourceLoaderBase.h"
#include "FTFontConverters.h"
#include "GLFontConverters.h"

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
		if (types[RESOURCE_FT_FONT]) addLoader(new FTFontConverters::ConvertFromFile());
		if (types[RESOURCE_GL_FONT]) addLoader(new GLFontConverters::ConvertFromFT());
	}

};

#endif //__RESOURCES_FONTCONVERTER_H__
