#pragma once

#ifndef __RESOURCES_TEXTURECONVERTER_H__
#define __RESOURCES_TEXTURECONVERTER_H__

#include "resources/ResourceLoaderBase.h"
//#include "GLTextureConverters.h"

class TextureConverter
	: public ResourceLoaderBase
{
public:
	/**
	* Constructs a font converter.
	* @param types the types of font resource loaders to support.
	*/
	TextureConverter()
	{
// 		addLoader(new GLTextureConverters::ConvertFromFile);
	}

};

#endif //__RESOURCES_TEXTURECONVERTER_H__
