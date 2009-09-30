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
	FontConverter()
	{
		addLoader(new FTFontConverters::ConvertFromFile);
		addLoader(new GLFontConverters::ConvertFromFT);
	}

};

#endif //__RESOURCES_FONTCONVERTER_H__
