//////////////////////////////////////////////////////////////////////////
//
// This file is part of Shinzui.
//
// Shinzui is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shinzui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shinzui.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////
//
// FileResourceLoader.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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
