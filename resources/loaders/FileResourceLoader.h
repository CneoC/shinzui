#pragma once

#ifndef __RESOURCES_FILERESOURCELOADER_H__
#define __RESOURCES_FILERESOURCELOADER_H__

#include "resources/ResourceLoaderBase.h"
#include "resources/FileResource.h"
#include "resources/ResourceTypes.h"

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

class FileResourceLoader
	: public ResourceLoaderBase
{
public:
	/**
	 * Constructs a new file resource loader for raw disk access resources.
	 */
	FileResourceLoader(const fs::path &base = "./")
		: m_basePath(base)
	{
	}

	virtual Resource get(const std::string &id, ResourceType type = RESOURCE_NULL)
	{
		fs::path p((m_basePath / id).native_directory_string());

		if (fs::exists(p))
		{
			FileData *pData = new FileData(this);
			pData->setId(id);
			pData->setPath(p);
			return FileResource(pData);
		}
		return Resource();
	}

	/**
	 * Loads a file resource.
	 * @return if resource was properly loaded.
	 */
	virtual bool load(Resource &res, u32 flags)
	{
		if (ResourceLoaderBase::load(res, flags))
			return true;

		FileResource file = res;
		res->setLoaded(fs::exists(file->getPath()));
		return res->isLoaded();
	}

	/**
	 * Unloads a resource (blocking).
	 */
	virtual bool unload(Resource &res, u32 flags)
	{
		if (ResourceLoaderBase::unload(res, flags))
			return true;

		return true;
	}

protected:
	fs::path		m_basePath;
};

#endif //__RESOURCES_FILERESOURCELOADER_H__
