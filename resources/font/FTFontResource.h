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
