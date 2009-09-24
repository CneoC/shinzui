#pragma once

#ifndef __FONT_RESOURCE_H__
#define __FONT_RESOURCE_H__

#include "resources/Resource.h"

class FontData
	: public ResourceData
{
public:
	enum { TYPE = RESOURCE_FONT };

	FontData(u32 type = 0)
		: ResourceData(type | RESOURCE_FONT)
	{
	}
};

typedef ResourceRef<FontData>		FontResource;

#endif //__FONT_RESOURCE_H__
