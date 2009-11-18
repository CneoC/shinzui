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
// FTTextureConverters.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "FITextureConverters.h"

#include "resources/FileResource.h"
#include "resources/texture/FITextureResource.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

using namespace resources;
using namespace resources::converters;

//////////////////////////////////////////////////////////////////////////

Resource FITextureConverters::ConvertFromFile::convert(const Resource &res, const ResourceType &type)
{
	FileResource file(res, DONT_CONVERT);
	if (file && (type & FITextureData::getName() ||
		fipImage::identifyFIF(file->getPath().native_file_string().c_str()) != FIF_UNKNOWN))
	{
		FITextureData *pData = new FITextureData(this);
		pData->setId(ResourceId(FITextureData::getName(), res->getId().getName()));
		pData->setSource(res);
		return FITextureResource(pData);
	}
	return Resource();
}

bool FITextureConverters::ConvertFromFile::load(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::load(res, flags))
		return true;

	FileResource file = res->getSource();
	if (!file->isLoaded())
		return false;

	FITextureResource texture(res);
	fipImage &img = texture->getTexture();

	if (!img.load(file->getPath().native_file_string().c_str()))
	{
		return false;
	}

	texture->setLoaded(true);

	return true;
}

bool FITextureConverters::ConvertFromFile::unload(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::unload(res, flags))
		return true;

	FITextureResource texture(res);
	fipImage &img = texture->getTexture();

	img.clear();

	texture->setLoaded(false);

	return true;
}