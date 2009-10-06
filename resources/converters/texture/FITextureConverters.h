#pragma once

#ifndef __RESOURCES_FITEXTURECONVERTERS_H__
#define __RESOURCES_FITEXTURECONVERTERS_H__

#include "resources/ResourceLoaderBase.h"

namespace resources
{
namespace converters
{
	namespace FITextureConverters
	{
		/**
		 * Loader class that can convert file resources to FreeImage texture resources.
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

#endif //__RESOURCES_FITEXTURECONVERTERS_H__
