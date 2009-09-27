#pragma once

#ifndef __RESOURCES_FONT_FONTRESOURCE_H__
#define __RESOURCES_FONT_FONTRESOURCE_H__

#include "resources/Resource.h"

class FontData
	: public ResourceData
{
public:
	enum { TYPE = RESOURCE_FONT };

	FontData(ResourceLoaderBase *pData)
		: ResourceData(pData)
	{
		setType(TYPE);
	}
};

typedef ResourceRef<FontData>		FontResource;

#endif //__RESOURCES_FONT_FONTRESOURCE_H__
