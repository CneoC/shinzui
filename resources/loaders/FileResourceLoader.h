#pragma once

#ifndef __RESOURCES_FILERESOURCELOADER_H__
#define __RESOURCES_FILERESOURCELOADER_H__

#include "resources/ResourceLoaderBase.h"

#include <boost/filesystem.hpp>

//////////////////////////////////////////////////////////////////////////

namespace resources
{
namespace loaders
{
	/**
	 * Loader class that loads a file resource.
	 */
	class FileResourceLoader
		: public ResourceLoaderBase
	{
	public:
		/**
		 * Constructs a new file resource loader for raw disk access resources.
		 */
		FileResourceLoader(const boost::filesystem::path &base = "./")
			: m_basePath(base)
		{
		}

		//! @see ResourceLoaderBase::get
		virtual Resource get(const ResourceId &id);

		//! @see ResourceLoaderBase::load
		virtual bool load(Resource &res, u32 flags);

		//! @see ResourceLoaderBase::unload
		virtual bool unload(Resource &res, u32 flags);

	protected:
		boost::filesystem::path	m_basePath;		// root path, pre-pended to all file names.
	};
}
}

#endif //__RESOURCES_FILERESOURCELOADER_H__
