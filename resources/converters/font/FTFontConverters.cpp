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
// FTFontConverters.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "FTFontConverters.h"

#include "os/current/Exception.h"

#include "resources/FileResource.h"
#include "resources/font/FTFontResource.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

using namespace resources;
using namespace resources::converters;

//////////////////////////////////////////////////////////////////////////

Resource FTFontConverters::ConvertFromFile::convert(const Resource &res, const ResourceType &type)
{
	FileResource file(res, DONT_CONVERT);
	if (file && (type & FTFontData::getName() || file->getPath().extension() == ".ttf"))
	{
		FTFontData *pData = new FTFontData(this);
		pData->setId(ResourceId(FTFontData::getName(), res->getId().getName()));
		pData->setSource(res);
		return FTFontResource(pData);
	}
	return Resource();
}

bool FTFontConverters::ConvertFromFile::load(Resource &res, u32 flags)
{
	// Try default load handling
	if (ResourceLoaderBase::load(res, flags))
		return true;

	FileResource file = res->getSource();
	if (!file->isLoaded())
		return false;

	FTFontResource font(res);

	if (FT_Init_FreeType(&font->getLibrary()))
	{
		THROW_EXCEPTION("FT_Init_FreeType failed.");
	}

	if (FT_New_Face(font->getLibrary(), file->getPath().native_file_string().c_str(), 0, &font->getFace()))
	{
		THROW_EXCEPTION((std::string("FT_New_Face failed. file '") + font->getId().toString() + "' probably doesn't exist.").c_str());
	}

	font->setSize(font->getSize(), font->getResolution());

	font->setLoaded(true);

	return true;
}

bool FTFontConverters::ConvertFromFile::unload(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::unload(res, flags))
		return true;

	FTFontResource font(res);

	font->setLoaded(false);

	// Done with freetype
	FT_Done_Face(font->getFace());
	FT_Done_FreeType(font->getLibrary());

	return true;
}
