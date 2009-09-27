#pragma once

#ifndef __RESOUCRES_LOADERS_FONT_FILEFTFONTLOADER_H__
#define __RESOUCRES_LOADERS_FONT_FILEFTFONTLOADER_H__

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

	virtual bool canConvert(Resource &res)
	{
		return false;
	}

	virtual bool convert(Resource &res)
	{
		return false;
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

#endif //__RESOUCRES_LOADERS_FONT_FILEFTFONTLOADER_H__
