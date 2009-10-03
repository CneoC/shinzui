#pragma once

#ifndef __TEST_STREAMRESOURCETEST_H__
#define __TEST_STREAMRESOURCETEST_H__

#include "core/Process.h"

#include "resources/RawResource.h"
#include "resources/ResourceLoaderBase.h"
#include "resources/ResourceType.h"

class StreamResourceTestData
	: public RawData
{
public:
	static const char *getName()	{ return "Test"; }

	StreamResourceTestData(ResourceLoaderBase *pData)
		: RawData(pData)
	{
		getType() += getName();
	}
};

typedef ResourceRef<StreamResourceTestData>		StreamResourceTestResource;

class StreamResourceTestLoader
	: public ResourceLoaderBase
{
public:
	/**
	 * Constructs a test resource loader for async loading testing purposes.
	 */
	StreamResourceTestLoader()
	{
	}

	virtual Resource get(const ResourceId &id)
	{
		if (id.getType() & "Test")
		{
			StreamResourceTestData *pData = new StreamResourceTestData(this);
			pData->setId(id);
			return StreamResourceTestResource(pData);
		}
		return Resource();
	}

	/**
	* Loads a file resource.
	* @return if resource was properly loaded.
	*/
	virtual bool load(Resource &res, u32 flags)
	{
		if (ResourceLoaderBase::load(res, flags))
			return true;

		// Wait for it...
		for (u32 i = 0; i < 100000; i++)
		{
			boost::this_thread::yield();
		}
		
		res->setLoaded(true);

		return res->isLoaded();
	}

	/**
	 * Unloads a resource (blocking).
	 */
	virtual bool unload(Resource &res, u32 flags)
	{
		if (ResourceLoaderBase::unload(res, flags))
			return true;

		return true;
	}
};

#endif //__TEST_STREAMRESOURCETEST_H__
