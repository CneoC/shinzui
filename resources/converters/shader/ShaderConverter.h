//////////////////////////////////////////////////////////////////////////
//
// This file is part of Shinzui.
//
// Shinzui is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shinzui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shinzui.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////
//
// ShaderConverter.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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
