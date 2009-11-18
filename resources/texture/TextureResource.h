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
// TextureResource.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __TEXTURE_RESOURCE_H__
#define __TEXTURE_RESOURCE_H__

#include "resources/Resource.h"

#include "math/Vector2.h"

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class TextureData
		: public ResourceData
	{
	public:
		static const char *getName()	{ return "Texture"; }

		//////////////////////////////////////////////////////////////////////////

		TextureData(ResourceLoaderBase *pData)
			: ResourceData(pData)
		{
			getType() += getName();
		}

		//////////////////////////////////////////////////////////////////////////

		virtual void bind() {}
		virtual void unbind() {}
	};

	//////////////////////////////////////////////////////////////////////////

	typedef ResourceRef<TextureData>	TextureResource;
}

#endif //__TEXTURE_RESOURCE_H__
