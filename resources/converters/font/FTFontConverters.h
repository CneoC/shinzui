#pragma once

#ifndef __RESOURCES_FTFONTCONVERTERS_H__
#define __RESOURCES_FTFONTCONVERTERS_H__

#include "resources/ResourceLoaderBase.h"

namespace resources {
namespace converters {
	namespace FTFontConverters
	{
		/**
		 * Loader class that can convert file font resources to FreeType2 font resources.
		 */
		class ConvertFromFile
			: public ResourceLoaderBase
		{
		public:
			//! @see ResourceLoaderBase::convert
			virtual Resource convert(const Resource &res, const ResourceType &type);

			//! @see ResourceLoaderBase::load
			virtual bool load(Resource &res, u32 flags);

			//! @see ResourceLoaderBase::unload
			virtual bool unload(Resource &res, u32 flags);
		};
	}
}
}

#endif //__RESOURCES_FTFONTCONVERTERS_H__
