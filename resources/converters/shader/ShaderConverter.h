#pragma once

#ifndef __RESOURCES_SHADERCONVERTER_H__
#define __RESOURCES_SHADERCONVERTER_H__

#include "resources/ResourceLoaderBase.h"
#include "GLShaderConverters.h"
#include "GLProgramConverters.h"

namespace resources
{
namespace converters
{
	class ShaderConverter
		: public ResourceLoaderBase
	{
	public:
		/**
		 * Constructs a font converter.
		 */
		ShaderConverter()
		{
			addLoader(new GLShaderConverters::ConvertFromFile);
			addLoader(new GLProgramConverters::ConvertFromProgram);
		}

	};
}
}

#endif //__RESOURCES_SHADERCONVERTER_H__
