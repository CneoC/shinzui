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
// GLFontConverters.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCES_GLFONTCONVERTERS_H__
#define __RESOURCES_GLFONTCONVERTERS_H__

#include "resources/ResourceLoaderBase.h"

#include "resources/font/GLFontResource.h"

namespace resources
{
namespace converters
{
	namespace GLFontConverters
	{
		/**
		 * Loader class that can convert FreeType2 font resources to OpenGL font resources.
		 */
		class ConvertFromFT
			: public ResourceLoaderBase
		{
		public:
			//! @see ResourceLoaderBase::convert
			virtual Resource convert(const Resource &res, const ResourceType &type);

			//! @see ResourceLoaderBase::load
			virtual bool load(Resource &res, u32 flags);

			//! @see ResourceLoaderBase::unload
			virtual bool unload(Resource &res, u32 flags);

		protected:
			/**
			 * Creates a display list entry for a font resource character.
			 * @param font	The font to create the display list for.
			 * @param c		The character to generate.
			 */
			void createDisplayList(GLFontResource &font, unsigned char c);
		};
	}
}
}

#endif //__RESOURCES_GLFONTCONVERTERS_H__
