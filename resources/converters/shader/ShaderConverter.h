#pragma once

#ifndef __RESOURCES_SHADERCONVERTER_H__
#define __RESOURCES_SHADERCONVERTER_H__

#include "resources/ResourceLoaderBase.h"
#include "GLShaderConverters.h"
#include "GLProgramConverters.h"

class ShaderConverter
	: public ResourceLoaderBase
{
public:
	/**
	 * Constructs a font converter.
	 * @param types the types of font resource loaders to support.
	 */
	ShaderConverter()
	{
		addLoader(new GLShaderConverters::ConvertFromFile);
		addLoader(new GLProgramConverters::ConvertFromFile);
	}

};

#endif //__RESOURCES_SHADERCONVERTER_H__
