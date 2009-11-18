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
// TextureConverter.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCES_TEXTURECONVERTER_H__
#define __RESOURCES_TEXTURECONVERTER_H__

#include "resources/ResourceLoaderBase.h"

#include "FITextureConverters.h"
#include "GLTextureConverters.h"
#include "GLFrameBufferConverters.h"

namespace resources
{
namespace converters
{
	class TextureConverter
		: public ResourceLoaderBase
	{
	public:
		/**
		* Constructs a font converter.
		*/
		TextureConverter()
		{
			addLoader(new FITextureConverters::ConvertFromFile);

			addLoader(new GLTextureConverters::ConvertFromFI);
			addLoader(new GLTextureConverters::ConvertFromFB);

			addLoader(new GLFrameBufferConverters::ConvertFromDef);
		}

	};
}
}

#endif //__RESOURCES_TEXTURECONVERTER_H__
