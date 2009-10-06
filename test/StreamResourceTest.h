#pragma once

#ifndef __TEST_STREAMRESOURCETEST_H__
#define __TEST_STREAMRESOURCETEST_H__

#include "core/Process.h"

#include "resources/RawResource.h"
#include "resources/ResourceLoaderBase.h"
#include "resources/ResourceType.h"

class StreamResourceTestData
	: public resources::RawData
{
public:
	static const char *getName()	{ return "Test"; }

	StreamResourceTestData(resources::ResourceLoaderBase *pData)
		: resources::RawData(pData)
	{
		getType() += getName();
	}
};

typedef resources::ResourceRef<StreamResourceTestData>		StreamResourceTestResource;

class StreamResourceTestLoader
	: public resources::ResourceLoaderBase
{
public:
	/**
	 * Constructs a test resource loader for async loading testing purposes.
	 */
	StreamResourceTestLoader()
	{
	}

	virtual resources::Resource get(const resources::ResourceId &id)
	{
		if (id.getType() & "Test")
		{
			StreamResourceTestData *pData = new StreamResourceTestData(this);
			pData->setId(id);
			return StreamResourceTestResource(pData);
		}
		return resources::Resource();
	}

	/**
	* Loads a file resource.
	* @return if resource was properly loaded.
	*/
	virtual bool load(resources::Resource &res, u32 flags)
	{
		if (resources::ResourceLoaderBase::load(res, flags))
			return true;

		// Wait for it...
		u32 count = 100000 * (1 + (rand() & (64-1)));
		for (u32 i = 0; i < count; i++)
		{
			volatile int j = 0;
		}
		
		res->setLoaded(true);

		return res->isLoaded();
	}

	/**
	 * Unloads a resource (blocking).
	 */
	virtual bool unload(resources::Resource &res, u32 flags)
	{
		if (resources::ResourceLoaderBase::unload(res, flags))
			return true;

		return true;
	}
};

#endif //__TEST_STREAMRESOURCETEST_H__
