#pragma once

#ifndef __TEXTURE_RESOURCE_H__
#define __TEXTURE_RESOURCE_H__

#include "resources/Resource.h"

#include "math/Vector2.h"

class TextureData
	: public ResourceData
{
public:
	static const char *getName()	{ return "Texture"; }

	TextureData(ResourceLoaderBase *pData)
		: ResourceData(pData)
	{
		getType() += getName();
	}

	virtual void bind() {}
	virtual void unbind() {}
};

typedef ResourceRef<TextureData>	TextureResource;

#endif //__TEXTURE_RESOURCE_H__
