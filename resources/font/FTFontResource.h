#pragma once

#ifndef __FT_FONT_RESOURCE_H__
#define __FT_FONT_RESOURCE_H__

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

protected:
	FT_Library	m_library;
	FT_Face		m_face;
};

typedef ResourceRef<FTFontData>		FTFontResource;

#endif //__FT_FONT_RESOURCE_H__
