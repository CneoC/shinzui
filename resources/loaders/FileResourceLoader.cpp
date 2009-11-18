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
// FileResourceLoader.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "FileResourceLoader.h"

#include "resources/Resource.h"
#include "resources/FileResource.h"

namespace fs = boost::filesystem;

using namespace resources;
using namespace resources::loaders;

//////////////////////////////////////////////////////////////////////////

Resource FileResourceLoader::get(const ResourceId &id)
{
	if (id.getType() & "File")
	{
		fs::path p((m_basePath / id.getName()).native_directory_string());

		if (fs::exists(p))
		{
			FileData *pData = new FileData(this);
			pData->setId(id);
			pData->setPath(p);
			return FileResource(pData);
		}
	}
	return Resource();
}

bool FileResourceLoader::load(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::load(res, flags))
		return true;

	FileResource file = res;
	res->setLoaded(fs::exists(file->getPath()));
	return res->isLoaded();
}

bool FileResourceLoader::unload(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::unload(res, flags))
		return true;

	return true;
}
