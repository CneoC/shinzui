#pragma once

#ifndef __RESOURCES_RAWRESOURCE_H__
#define __RESOURCES_RAWRESOURCE_H__

#include "Resource.h"

class RawData
	: public ResourceData
{
public:
	enum { TYPE = RESOURCE_RAW };

	RawData(ResourceLoaderBase *pData)
		: ResourceData(pData)
	{
		setType(TYPE);
	}
};

typedef ResourceRef<RawData>			RawResource;

#endif //__RESOURCES_RAWRESOURCE_H__
