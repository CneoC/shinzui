#pragma once

#ifndef __RESOURCES_FONT_FTFONTRESOURCE_H__
#define __RESOURCES_FONT_FTFONTRESOURCE_H__

#include "FontResource.h"

class FTFontData
	: public FontData
{
public:
	enum { TYPE = RESOURCE_FT_FONT };

	FTFontData(ResourceLoaderBase *pData)
		: FontData(pData)
	{
		setType(TYPE);
	}

	FT_Library &getLibrary()	{ return m_library; }
	FT_Face &getFace()			{ return m_face; }

	void setSize(int size, int resolution = 128)
	{ 
		FT_Set_Char_Size(m_face, size << 6, size << 6, resolution, resolution);
	}

protected:
	FT_Library	m_library;
	FT_Face		m_face;
};

typedef ResourceRef<FTFontData>		FTFontResource;

#endif //__RESOURCES_FONT_FTFONTRESOURCE_H__
