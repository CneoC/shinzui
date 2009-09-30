#pragma once

#ifndef __RESOURCES_FONT_FONTRESOURCE_H__
#define __RESOURCES_FONT_FONTRESOURCE_H__

#include "resources/Resource.h"

class FontData
	: public ResourceData
{
public:
	static const char *getName()	{ return "Font"; }

	FontData(ResourceLoaderBase *pData)
		: ResourceData(pData)
	{
		getType() += getName();
	}
};

typedef ResourceRef<FontData>		FontResource;

#endif //__RESOURCES_FONT_FONTRESOURCE_H__
