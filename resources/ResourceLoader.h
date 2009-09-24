#pragma once

#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

#include "resources/Resource.h"

class ResourceLoader
{
public:
	typedef std::list<ResourceLoader *>	LoaderList;

	virtual Resource load(const char *path, ResourceType type = RESOURCE_NULL, ResourceCache *pCache = NULL)
	{
		Resource result;
		LoaderList::iterator iter;
		for (iter = m_loaders.begin(); iter != m_loaders.end(); ++iter)
		{
			result = (*iter)->load(path, type, pCache);
			if (result)
			{
				if (pCache) pCache->put(path, result);
				break;
			}
		}
		return result;
	}

	TextureResource loadTexture(const char *path, ResourceCache *pCache = NULL)
	{
		return load(path, RESOURCE_TEXTURE, pCache);
	}

	ShaderResource loadShader(const char *path, ResourceCache *pCache = NULL)
	{
		return load(path, RESOURCE_SHADER, pCache);
	}

	FontResource loadFont(const char *path, ResourceCache *pCache = NULL)
	{
		return load(path, RESOURCE_FONT, pCache);
	}

	void addLoader(ResourceLoader *pLoader)		{ m_loaders.push_front(pLoader); }
	void removeLoader(ResourceLoader *pLoader)	{ m_loaders.remove(pLoader); }
	void clearLoaders()							{ m_loaders.clear(); }

protected:
	LoaderList	m_loaders;
};

class FSResourceLoader
	: public ResourceLoader
{
public:
	virtual Resource load(const char *path, ResourceType = RESOURCE_NULL, ResourceCache *pCache = NULL)
	{
		return Resource();
	}
};

#endif //__RESOURCE_LOADER_H__
