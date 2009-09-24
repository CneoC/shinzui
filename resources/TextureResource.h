#pragma once

#ifndef __TEXTURE_RESOURCE_H__
#define __TEXTURE_RESOURCE_H__

#include "resources/Resource.h"

class TextureData
	: public ResourceData
{
public:
	enum { TYPE = RESOURCE_TEXTURE };

	TextureData(u32 type = 0)
		: ResourceData(type | TYPE)
	{
	}
};

typedef ResourceRef<TextureData>	TextureResource;

#endif //__TEXTURE_RESOURCE_H__
