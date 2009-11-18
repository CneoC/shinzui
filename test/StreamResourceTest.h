//////////////////////////////////////////////////////////////////////////
//
// This file is part of Shinzui.
//
// Shinzui is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shinzui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shinzui.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////
//
// StreamResourceTest.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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
