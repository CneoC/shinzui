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
		FLAG_DONT_RECURSE		= 1 << 1,		// Mark the load (or unload) as none recursive (Resource::getSource())

		ASYNC_PRIORITY_MASK		= 0xFF000000,	// Mask async priority value
		ASYNC_PRIORITY_SHIFT	= 24,			// Shift async priority value

		ASYNC_PRIORITY_CRITICAL	= 255 << ASYNC_PRIORITY_SHIFT,
		ASYNC_PRIORITY_HIGH		= 128 << ASYNC_PRIORITY_SHIFT,
		ASYNC_PRIORITY_NORMAL	= 64 << ASYNC_PRIORITY_SHIFT,
		ASYNC_PRIORITY_LOW		= 1 << ASYNC_PRIORITY_SHIFT,

		FLAG_LOAD_DEFAULT		= FLAG_NONE,
		FLAG_UNLOAD_DEFAULT		= FLAG_DONT_RECURSE
	};

	ResourceLoaderBase()
		: m_pParent(NULL)
	{}

	/**
	 * Gets a resource object by name (possibly not yet loaded).
	 * @param id	Identifier of the resource.
	 * @param type	Resource type used as a hint for loaders (some loaders might require this!)
	 * @return The requested resource (a NULL resource when unable to load).
	 */
	virtual Resource get(const std::string &id, ResourceType type = RESOURCE_NULL)
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

	/**
	 * Converts a resource from one type to another if possible.
	 * @param resource	The resource to convert.
	 * @param type		The type to convert the resource to.
	 */
	virtual Resource convert(const Resource &res, ResourceType type)
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

	//! Gets a texture resource definition.
	TextureResource getTexture(const std::string &id)
	{
		return get(id, RESOURCE_TEXTURE);
	}

	//! Gets a shader resource definition.
	ShaderResource getShader(const std::string &id)
	{
		return get(id, RESOURCE_SHADER);
	}

	//! Gets a font resource definition.
	FontResource getFont(const std::string &id)
	{
		return get(id, RESOURCE_FONT);
	}

	/**
	 * Loads a resource (blocking).
	 * @return if resource was properly loaded.
	 */
	virtual bool load(Resource &res, u32 flags = FLAG_LOAD_DEFAULT)
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

	/**
	 * Unloads a resource (blocking).
	 */
	virtual bool unload(Resource &res, u32 flags = FLAG_UNLOAD_DEFAULT)
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

	//! Registers a child loader to manage more specific loading.
	void addLoader(const SharedPtr &loader)		{ m_loaders.push_front(loader); loader->setParent(this); }
	//! Registers a child loader and puts it in a ResourceLoaderRef to manage more specific loading.
	void addLoader(ResourceLoaderBase *pLoader)	{ addLoader(SharedPtr(pLoader)); }
	//! Removes a child loader.
	void removeLoader(const SharedPtr &loader)	{ m_loaders.remove(loader); }
	//! Clear all child loaders
	void clearLoaders()							{ m_loaders.clear(); }

	ResourceLoaderBase *getRoot()				{ return m_pParent? m_pParent->getRoot(): this; }
	ResourceLoaderBase *getParent() const		{ return m_pParent; }

protected:
	void setParent(ResourceLoaderBase *pParent) { m_pParent = pParent; }

	ResourceLoaderBase *m_pParent;
	LoaderList			m_loaders;
};

#endif //__RESOURCE_LOADER_BASE_H__
