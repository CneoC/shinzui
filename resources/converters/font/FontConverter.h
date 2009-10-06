#pragma once

#ifndef __RESOURCES_FONTCONVERTER_H__
#define __RESOURCES_FONTCONVERTER_H__

#include "resources/ResourceLoaderBase.h"
#include "FTFontConverters.h"
#include "GLFontConverters.h"

namespace resources {
namespace converters {
	class FontConverter
		: public ResourceLoaderBase
	{
	public:
		/**
		 * Constructs a font converter.
		 */
		FontConverter()
		{
			addLoader(new FTFontConverters::ConvertFromFile);
			addLoader(new GLFontConverters::ConvertFromFT);
		}

	};
}
}

#endif //__RESOURCES_FONTCONVERTER_H__
