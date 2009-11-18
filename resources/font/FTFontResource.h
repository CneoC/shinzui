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
// FTFontResource.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCES_FONT_FTFONTRESOURCE_H__
#define __RESOURCES_FONT_FTFONTRESOURCE_H__

#include "FontResource.h"

#include <ft2build.h>
#include <freetype/freetype.h>

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class FTFontData
		: public FontData
	{
	public:
		static const char *getName()	{ return "FTFont"; }

		//////////////////////////////////////////////////////////////////////////

		FTFontData(ResourceLoaderBase *pData)
			: FontData(pData)
			, m_face(NULL)
			, m_library(NULL)
			, m_size(14)
			, m_resolution(128)
		{
			getType() += getName();
		}

		//////////////////////////////////////////////////////////////////////////

		FT_Library &getLibrary()	{ return m_library; }
		FT_Face &getFace()			{ return m_face; }

		u32 getSize() const			{ return m_size; }
		u32 getResolution() const	{ return m_resolution; }

		void setSize(u32 size, u32 resolution = 128)
		{ 
			m_size = size;
			m_resolution = resolution;

			if (m_face) FT_Set_Char_Size(m_face, m_size << 6, m_size << 6, m_resolution, m_resolution);
		}

	protected:
		u32			m_size;
		u32			m_resolution;

		FT_Library	m_library;
		FT_Face		m_face;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef ResourceRef<FTFontData>		FTFontResource;
}

#endif //__RESOURCES_FONT_FTFONTRESOURCE_H__
