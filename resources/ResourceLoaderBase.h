#pragma once

#ifndef __RESOURCE_LOADER_BASE_H__
#define __RESOURCE_LOADER_BASE_H__

#include "Resource.h"
#include "ResourceCache.h"

#include "texture/TextureResource.h"
#include "shader/ShaderResource.h"
#include "font/FontResource.h"

#include <list>

#include <boost/shared_ptr.hpp>

class ResourceLoaderBase
{
public:
	typedef boost::shared_ptr<ResourceLoaderBase>	SharedPtr;
	typedef std::list<SharedPtr>					LoaderList;

	enum LoadFlags
	{
		FLAG_NONE,
		FLAG_ASYNC				= 1 << 0,		// Mark the load (or unload) as asynchronous

		ASYNC_PRIORITY_MASK		= 0xFF000000,	// Mask async priority value
		ASYNC_PRIORITY_SHIFT	= 24,			// Shift async priority value

		ASYNC_PRIORITY_CRITICAL	= 255 << ASYNC_PRIORITY_SHIFT,
		ASYNC_PRIORITY_HIGH		= 128 << ASYNC_PRIORITY_SHIFT,
		ASYNC_PRIORITY_NORMAL	= 64 << ASYNC_PRIORITY_SHIFT,
		ASYNC_PRIORITY_LOW		= 1 << ASYNC_PRIORITY_SHIFT,
	};

	/**
	 * Gets a resource object (possibly not yet loaded)
	 * @return the requested resource (a NULL resource when unable to load)
	 */
	virtual Resource get(const std::string &id, ResourceType type = RESOURCE_NULL)
	{
		LoaderList::iterator iter;
		for (iter = m_loaders.begin(); iter != m_loaders.end(); ++iter)
		{
			Resource result = (*iter)->get(id, type);
			if (result)
				return result;
		}
		return Resource();
	}

	//! Gets a texture resource definition (possibly not yet loaded)
	TextureResource getTexture(const std::string &id)
	{
		return get(id, RESOURCE_TEXTURE);
	}

	//! Gets a shader resource definition (possibly not yet loaded)
	ShaderResource getShader(const std::string &id)
	{
		return get(id, RESOURCE_SHADER);
	}

	//! Gets a font resource definition (possibly not yet loaded)
	FontResource getFont(const std::string &id)
	{
		return get(id, RESOURCE_FONT);
	}

	/**
	 * Loads a resource (blocking).
	 * @return if resource was properly loaded.
	 */
	virtual bool load(Resource &res, u32 flags = FLAG_NONE)
	{
		if (res->isLoaded())
			return true;

		if (flags & FLAG_ASYNC)
		{
			res->setLoad(true);
			return true;
		}
		return false;
	}

	/**
	 * Unloads a resource (blocking).
	 */
	virtual bool unload(Resource &res, u32 flags = FLAG_NONE)
	{
		if (!res->isLoaded())
			return true;

		if (flags & FLAG_ASYNC)
		{
			res->setUnload(true);
			return true;
		}
		return false;
	}

	//! Registers a child loader and puts it in a ResourceLoaderRef to manage more specific loading.
	void addLoader(ResourceLoaderBase *pLoader)	{ m_loaders.push_front(SharedPtr(pLoader)); }
	//! Registers a child loader to manage more specific loading.
	void addLoader(const SharedPtr &loader)		{ m_loaders.push_front(loader); }
	//! Removes a child loader.
	void removeLoader(const SharedPtr &loader)	{ m_loaders.remove(loader); }
	//! Clear all child loaders
	void clearLoaders()							{ m_loaders.clear(); }

protected:
	LoaderList	m_loaders;
};

#endif //__RESOURCE_LOADER_BASE_H__
