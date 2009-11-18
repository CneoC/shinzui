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
// FileResource.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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
