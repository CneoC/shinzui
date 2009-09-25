#pragma once

#ifndef __FILE_FT_FONT_LOADER_H__
#define __FILE_FT_FONT_LOADER_H__

#include "resources/ResourceLoaderBase.h"

#include "resources/font/FTFontResource.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

class FileFTFontLoader
	: public ResourceLoaderBase
{
public:
	/**
	 * Constructs a freetype2 font loader
	 */
	FileFTFontLoader()
	{
	}

	virtual Resource get(const std::string &id, ResourceType type = RESOURCE_NULL)
	{
		fs::path p(id);
		
		if (fs::exists(p) && p.extension() == ".ttf")
		{
			FTFontData *pData = new FTFontData(this);
			pData->setId(id);
			return FTFontResource(pData);
		}
		return Resource();
	}

	/**
	 * Loads the freetype2 font resource (blocking).
	 * @return if resource was properly loaded.
	 */
	virtual bool load(Resource &res, u32 flags = FLAG_NONE)
	{
		if (ResourceLoaderBase::load(res, flags))
			return true;

		FTFontResource font(res);
	
		if (FT_Init_FreeType(&font->getLibrary()))
		{
			throw std::runtime_error("FT_Init_FreeType failed.");
		}

		if (FT_New_Face(font->getLibrary(), font->getId().c_str(), 0, &font->getFace()))
		{
			throw std::runtime_error(std::string("FT_New_Face failed. file '") + font->getId() + "' probably doesn't exist.");
		}

		FT_Set_Char_Size(font->getFace(), 16 << 6, 16 << 6, 96, 96);
	
		font->setLoaded(true);

		return true;
	}

	/**
	 * Unloads a resource (blocking).
	 */
	virtual bool unload(Resource &res, u32 flags = FLAG_NONE)
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

};

#endif //__FS_FONT_LOADER_H__
