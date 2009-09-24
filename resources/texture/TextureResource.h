#pragma once

#ifndef __TEXTURE_RESOURCE_H__
#define __TEXTURE_RESOURCE_H__

#include "resources/Resource.h"

class TextureData
	: public ResourceData
{
public:
	enum { TYPE = RESOURCE_TEXTURE };

	TextureData()
	{
		setType(TYPE);
	}
};

typedef ResourceRef<TextureData>	TextureResource;

#endif //__TEXTURE_RESOURCE_H__
