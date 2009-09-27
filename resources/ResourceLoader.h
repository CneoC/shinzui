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
		, m_log(LOG_GET("Resource.Loader"))
	{
	}

	/**
	 * @see ResourceLoaderBase::find
	 */
	virtual Resource get(const std::string &id, ResourceType type = RESOURCE_NULL)
	{
		LOG_INFO(m_log, '\'' << id << '\'');

		Resource result;
		if (m_pCache)
		{
			result = m_pCache->find(id, type);
			if (result) return result;
		}
		result = ResourceLoaderBase::get(id, type);
		if (m_pCache && result)
			m_pCache->add(result);
		return result;
	}

	virtual Resource convert(const Resource &resource, ResourceType type)
	{
		LOG_INFO(m_log, '\'' << resource->getId() << '\'');

		Resource result;
		if (m_pCache)
		{
			result = m_pCache->find(resource->getId(), type);
			if (result) return result;
		}
		result = ResourceLoaderBase::convert(resource, type);
		if (m_pCache && result)
			m_pCache->add(result);
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
	ResourceCache *	m_pCache;
	logging::Log *	m_log;
};

#endif //__RESOURCE_LOADER_H__
