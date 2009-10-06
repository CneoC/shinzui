#pragma once

#ifndef __RESOURCES_FILERESOURCE_H__
#define __RESOURCES_FILERESOURCE_H__

#include "RawResource.h"

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

namespace resources
{
	/**
	 * File resource data.
	 * TODO: instead of just representing a path, implement file reading functions?
	 */
	class FileData
		: public RawData
	{
	public:
		static const char *getName()	{ return "File"; }

		//////////////////////////////////////////////////////////////////////////

		FileData(ResourceLoaderBase *pData)
			: RawData(pData)
		{
			getType() += getName();
		}

		//////////////////////////////////////////////////////////////////////////

		void setPath(const fs::path &path)	{ m_path = path; }
		const fs::path &getPath() const		{ return m_path; }

	protected:
		fs::path	m_path;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef ResourceRef<FileData>		FileResource;
}

#endif //__RESOURCES_FILERESOURCE_H__
