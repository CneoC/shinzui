#pragma once

#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

#include "ResourceLoaderBase.h"

#include "util/logging/Log.h"

class ResourceCache;

class ResourceLoader
	: public ResourceLoaderBase
{
public:
	/**
	 * Constructs a base resource loader.
	 * @param pCache	the ResourceCache to use to manage cached and async loaded resources.
	 */
	ResourceLoader(ResourceCache *pCache = NULL);

	//////////////////////////////////////////////////////////////////////////

	/**
	 * @see ResourceLoaderBase::get
	 */
	virtual Resource get(const ResourceId &id);
	virtual Resource convert(const Resource &resource, const ResourceType &type);
	virtual void clone(const Resource &src, Resource dst);

	//////////////////////////////////////////////////////////////////////////

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
