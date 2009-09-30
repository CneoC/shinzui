#pragma once

#ifndef __TEXTURE_RESOURCE_H__
#define __TEXTURE_RESOURCE_H__

#include "resources/Resource.h"

class TextureData
	: public ResourceData
{
public:
	static const char *getName()	{ return "Texture"; }

	TextureData()
	{
		getType() += getName();
	}
};

typedef ResourceRef<TextureData>	TextureResource;

#endif //__TEXTURE_RESOURCE_H__
