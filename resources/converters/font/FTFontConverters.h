#pragma once

#ifndef __RESOURCES_FTFONTCONVERTERS_H__
#define __RESOURCES_FTFONTCONVERTERS_H__

#include "resources/ResourceLoaderBase.h"

#include "resources/font/FTFontResource.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

namespace FTFontConverters
{
	/**
	 * Loader class that can convert file font resources to freetype2 font resources.
	 */
	class ConvertFromFile
		: public ResourceLoaderBase
	{
		virtual Resource convert(const Resource &res, const ResourceType &type)
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

		/**
		 * Loads the freetype2 font resource (blocking).
		 * @return if resource was properly loaded.
		 */
		virtual bool load(Resource &res, u32 flags)
		{
			if (ResourceLoaderBase::load(res, flags))
				return true;

			FileResource file = res->getSource();
			if (!file->isLoaded())
				return false;

			FTFontResource font(res);

			if (FT_Init_FreeType(&font->getLibrary()))
			{
				throw std::runtime_error("FT_Init_FreeType failed.");
			}

			if (FT_New_Face(font->getLibrary(), file->getPath().native_file_string().c_str(), 0, &font->getFace()))
			{
				throw std::runtime_error(std::string("FT_New_Face failed. file '") + font->getId().toString() + "' probably doesn't exist.");
			}

			font->setSize(font->getSize(), font->getResolution());
		
			font->setLoaded(true);

			return true;
		}

		/**
		 * Unloads a resource (blocking).
		 */
		virtual bool unload(Resource &res, u32 flags)
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
}

#endif //__RESOURCES_FTFONTCONVERTERS_H__
