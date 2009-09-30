#include "ResourceLoaderBase.h"

#include "ResourceCache.h"

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
				if (!loadRes->isLoaded()) loadRes->setLoad(true);
				loadRes = loadRes->getSource();
			}
		}

		return true;
	}
	// Load recursively and blocking
	else if (!(flags & FLAG_DONT_RECURSE))
	{
		Resource loadRes = res->getSource();
		while (loadRes)
		{
			if (!loadRes->isLoaded()) loadRes.load(flags);
			loadRes = loadRes->getSource();
		}
	}

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

	return false;
}

void ResourceLoaderBase::clone(const Resource &src, Resource dst)
{
	bool loaded	= dst->isLoaded() && !dst->isUnloading();
	bool loadAsync = dst->isLoading();

	// Reset load/unload flags
	dst->resetFlags();

	// Use same source resource
	dst->setSource(src->getSource());

	if (loaded)
		load(dst, loadAsync? FLAG_ASYNC: FLAG_NONE);
}
