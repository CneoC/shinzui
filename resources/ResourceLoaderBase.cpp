#include "ResourceLoaderBase.h"

#include "ResourceCache.h"

Resource ResourceLoaderBase::get(const std::string &id, ResourceType type)
{
	// try if the child loaders can get the resource
	LoaderList::iterator iter;
	for (iter = m_loaders.begin(); iter != m_loaders.end(); ++iter)
	{
		Resource result = (*iter)->get(id, type);
		if (result)
			return result;
	}
	return Resource();
}

Resource ResourceLoaderBase::convert(const Resource &res, ResourceType type)
{
	// no conversion needed
	if (res.isType(type))
		return res;

	// try if the child loaders can convert the resource
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
	if (res->isLoaded())
		return true;

	if (flags & FLAG_ASYNC)
	{
		if (flags & FLAG_DONT_RECURSE)
		{
			if (!res->isLoaded()) res->setLoad(true);
		}
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
	if (!res->isLoaded())
		return true;

	if (flags & FLAG_ASYNC)
	{
		if (flags & FLAG_DONT_RECURSE)
		{
			if (res->isLoaded()) res->setUnload(true);
		}
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
