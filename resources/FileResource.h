#pragma once

#ifndef __RESOURCES_FILERESOURCE_H__
#define __RESOURCES_FILERESOURCE_H__

#include "RawResource.h"

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

class FileData
	: public RawData
{
public:
	enum { TYPE = RESOURCE_FILE };

	FileData(ResourceLoaderBase *pData)
		: RawData(pData)
	{
		setType(TYPE);
	}

	void setPath(const fs::path &path)	{ m_path = path; }
	const fs::path &getPath() const		{ return m_path; }

protected:
	fs::path	m_path;
};

typedef ResourceRef<FileData>		FileResource;

#endif //__RESOURCES_FILERESOURCE_H__
