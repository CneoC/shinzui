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
// ResourceConverters.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCES_RESOURCECONVERTERS_H__
#define __RESOURCES_RESOURCECONVERTERS_H__

#include "resources/ResourceLoaderBase.h"
#include "resources/converters/font/FontConverter.h"
#include "resources/converters/shader/ShaderConverter.h"
#include "resources/converters/texture/TextureConverter.h"
#include "resources/ResourceType.h"

namespace resources
{
namespace converters
{
	class ResourceConverters
		: public ResourceLoaderBase
	{
	public:
		/**
		* Constructs a new file resource loader for raw disk access resources.
		*/
		ResourceConverters()
		{
			addLoader(new FontConverter);
			addLoader(new ShaderConverter);
			addLoader(new TextureConverter);
		}
	};
}
}

#endif //__RESOURCES_RESOURCECONVERTERS_H__
