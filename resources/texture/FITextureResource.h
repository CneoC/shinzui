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
// FITextureResource.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCES_FITEXTURERESOURCE_H__
#define __RESOURCES_FITEXTURERESOURCE_H__

#include "TextureResource.h"

#include <FreeImagePlus.h>

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class FITextureData
		: public TextureData
	{
	public:
		static const char *getName()	{ return "FITexture"; }

		//////////////////////////////////////////////////////////////////////////

		FITextureData(ResourceLoaderBase *pData)
			: TextureData(pData)
		{
			getType() += getName();
		}

		//////////////////////////////////////////////////////////////////////////

		fipImage &getTexture()	{ return m_texture; }

	protected:
		fipImage	m_texture;
	};

	typedef ResourceRef<FITextureData>		FITextureResource;
}

#endif //__RESOURCES_FITEXTURERESOURCE_H__
