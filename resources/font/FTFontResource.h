#pragma once

#ifndef __RESOURCES_FONT_FTFONTRESOURCE_H__
#define __RESOURCES_FONT_FTFONTRESOURCE_H__

#include "FontResource.h"

#include <ft2build.h>
#include <freetype/freetype.h>

class FTFontData
	: public FontData
{
public:
	enum { TYPE = RESOURCE_FT_FONT };

	FTFontData(ResourceLoaderBase *pData)
		: FontData(pData)
		, m_face(NULL)
		, m_library(NULL)
		, m_size(14)
		, m_resolution(128)
	{
		setType(TYPE);
	}

	FT_Library &getLibrary()	{ return m_library; }
	FT_Face &getFace()			{ return m_face; }

	void setSize(int size, int resolution = 128)
	{ 
		m_size = size;
		m_resolution = resolution;

		if (m_face) FT_Set_Char_Size(m_face, m_size << 6, m_size << 6, m_resolution, m_resolution);
	}

protected:
	int			m_size;
	int			m_resolution;

	FT_Library	m_library;
	FT_Face		m_face;
};

typedef ResourceRef<FTFontData>		FTFontResource;

#endif //__RESOURCES_FONT_FTFONTRESOURCE_H__
