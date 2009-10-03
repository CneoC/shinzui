#pragma once

#ifndef __RESOURCES_TEXTURECONVERTER_H__
#define __RESOURCES_TEXTURECONVERTER_H__

#include "resources/ResourceLoaderBase.h"
#include "FITextureConverters.h"
#include "GLTextureConverters.h"
#include "GLFrameBufferConverters.h"

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
		addLoader(new FITextureConverters::ConvertFromFile);

		addLoader(new GLTextureConverters::ConvertFromFI);
		addLoader(new GLTextureConverters::ConvertFromFB);

		addLoader(new GLFrameBufferConverters::ConvertFromDef);
	}

};

#endif //__RESOURCES_TEXTURECONVERTER_H__
