#pragma once

#ifndef __RESOURCES_RAWRESOURCE_H__
#define __RESOURCES_RAWRESOURCE_H__

#include "Resource.h"

class RawData
	: public ResourceData
{
public:
	static const char *getName()	{ return "Raw"; }

	RawData(ResourceLoaderBase *pData)
		: ResourceData(pData)
	{
		getType() += getName();
	}
};

typedef ResourceRef<RawData>			RawResource;

#endif //__RESOURCES_RAWRESOURCE_H__
