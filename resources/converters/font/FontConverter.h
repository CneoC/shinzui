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
// FontConverter.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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
