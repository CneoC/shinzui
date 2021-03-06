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
// ResourceLoaderBase.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "ResourceLoaderBase.h"

#include "ResourceCache.h"

#include <stack>

using namespace resources;

//////////////////////////////////////////////////////////////////////////

Resource ResourceLoaderBase::get(const ResourceId &id)
{
	// Try to get the resource with each of the child loaders
	LoaderList::iterator iter;
	for (iter = m_loaders.begin(); iter != m_loaders.end(); ++iter)
	{
		Resource result = (*iter)->get(id);
		if (result)
			return result;
	}
	return Resource();
}

Resource ResourceLoaderBase::convert(const Resource &res, const ResourceType &type)
{
	// Try to convert the resource with each of the child loaders
	LoaderList::iterator iter;
	for (iter = m_loaders.begin(); iter != m_loaders.end(); ++iter)
	{
		Resource result = (*iter)->convert(res, type);
		if (result)
			return result;
	}
	return Resource();
}

bool ResourceLoaderBase::load(Resource &res, u32 flags)
{
	// Already loaded
	if (res->isLoaded())
		return true;

	// Asynchronous load requested
	if (flags & FLAG_ASYNC)
	{
		// Mark just this resource for loading
		if (flags & FLAG_DONT_RECURSE)
		{
			if (!res->isLoaded()) res->setLoad(true);
		}
		// Mark for loading recursively
		else
		{
			Resource loadRes = res;
			while (loadRes)
			{
				if (!loadRes->isLoaded())
					loadRes->setLoad(true);
				loadRes = loadRes->getSource();
			}
		}

		LOG_TRACE(LOG_GET("Resource.Loader"), "Async " << res->getId().toString());

		return true;
	}
	// Load recursively and blocking
	else if (!(flags & FLAG_DONT_RECURSE))
	{
		std::stack<Resource> loadStack;

		// For all sources
		Resource loadRes = res->getSource();
		while (loadRes)
		{
			// Add unloaded resources to load stack
			if (!loadRes->isLoaded())
				loadStack.push(loadRes);
			loadRes = loadRes->getSource();
		}

		// Load the resource dependencies in proper order
		while (!loadStack.empty())
		{
			loadStack.top().load(flags);
			loadStack.pop();
		}
	}

	LOG_INFO(LOG_GET("Resource.Loader"), res->getId().toString());

	return false;
}

bool ResourceLoaderBase::unload(Resource &res, u32 flags)
{		
	// Not loaded
	if (!res->isLoaded())
		return true;

	// Asynchronous unload requested
	if (flags & FLAG_ASYNC)
	{
		// Mark just this resource for unloading
		if (flags & FLAG_DONT_RECURSE)
		{
			if (res->isLoaded()) res->setUnload(true);
		}
		// Mark for unloading recursively
		else
		{
			Resource loadRes = res;
			while (loadRes)
			{
				if (loadRes->isLoaded()) loadRes->setUnload(true);
				loadRes = loadRes->getSource();
			}
		}

		LOG_TRACE(LOG_GET("Resource.Loader"), "Async " << res->getId().toString());

		return true;
	}
	// Unload recursively and blocking
	else if (!(flags & FLAG_DONT_RECURSE))
	{
		Resource loadRes = res->getSource();
		while (loadRes)
		{
			if (loadRes->isLoaded()) loadRes.unload(flags);
			loadRes = loadRes->getSource();
		}
	}

	LOG_INFO(LOG_GET("Resource.Loader"), res->getId().toString());

	return false;
}

Resource ResourceLoaderBase::clone(const Resource &src, Resource dst)
{
	bool loaded	= src->isLoaded() && !src->isUnloading();
	bool loadAsync = src->isLoading();

	// Reset load/unload flags
	dst->resetFlags();

	// Use same source resource
	dst->setSource(src->getSource());

	if (loaded)
		load(dst, loadAsync? FLAG_ASYNC: FLAG_NONE);

	return dst;
}
