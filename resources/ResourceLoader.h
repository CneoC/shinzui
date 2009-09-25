#pragma once

#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

#include "ResourceLoaderBase.h"

class ResourceLoader
	: public ResourceLoaderBase
{
public:
	/**
	 * Constructs a base resource loader.
	 * @param pCache	the ResourceCache to use to manage cached and async loaded resources.
	 */
	ResourceLoader(ResourceCache *pCache = NULL)
		: m_pCache(pCache)
	{
	}

	/**
	 * Gets a resource object (possibly not yet loaded)
	 * @return the requested resource (a NULL resource when unable to load)
	 */
	virtual Resource get(const std::string &id, ResourceType type = RESOURCE_NULL)
	{
		Resource result;
		if (m_pCache)
		{
			result = m_pCache->get(id);
			if (result) return result;
		}
		result = ResourceLoaderBase::get(id, type);
		if (m_pCache && result)
			m_pCache->put(id, result);
		return result;
	}

	/**
	 * Loads a resource (blocking).
	 * @return if resource was properly loaded.
	 */
	virtual bool load(Resource &res, u32 flags = FLAG_NONE)		{ return false; }

	/**
	 * Unloads a resource (blocking).
	 */
	virtual bool unload(Resource &res, u32 flags = FLAG_NONE)	{ return false; }

protected:
	ResourceCache*	m_pCache;
};

#endif //__RESOURCE_LOADER_H__
