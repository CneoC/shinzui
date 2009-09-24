#pragma once

#ifndef __FS_RESOURCE_LOADER_H__
#define __FS_RESOURCE_LOADER_H__

#include "FileResourceLoader.h"

#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

class FSResourceLoader
	: public FileResourceLoader
{
public:
	FSResourceLoader(ResourceTypeSet &types, const fs::path &base = "./")
		: FileResourceLoader(types)
		, m_basePath(base)
	{
	}

	virtual Resource get(const std::string &id, ResourceType type = RESOURCE_NULL)
	{
		return FileResourceLoader::get((m_basePath / id).native_directory_string(), type);
	}

protected:
	fs::path m_basePath;
};

#endif //__FS_RESOURCE_LOADER_H__
