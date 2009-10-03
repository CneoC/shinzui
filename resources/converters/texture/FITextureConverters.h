#pragma once

#ifndef __RESOURCES_FITEXTURECONVERTERS_H__
#define __RESOURCES_FITEXTURECONVERTERS_H__

#include "resources/ResourceLoaderBase.h"

#include "resources/texture/FITextureResource.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

namespace FITextureConverters
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

		/**
		 * Loads the free image texture resource (blocking).
		 * @return if resource was properly loaded.
		 */
		virtual bool load(Resource &res, u32 flags)
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

		/**
		 * Unloads a resource (blocking).
		 */
		virtual bool unload(Resource &res, u32 flags)
		{
			if (ResourceLoaderBase::unload(res, flags))
				return true;

			FITextureResource texture(res);
			fipImage &img = texture->getTexture();

			img.clear();

			texture->setLoaded(false);

			return true;
		}
	};
}

#endif //__RESOURCES_FITEXTURECONVERTERS_H__
