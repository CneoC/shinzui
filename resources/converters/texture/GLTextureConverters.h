#pragma once

#ifndef __RESOURCES_GLTEXTURECONVERTERS_H__
#define __RESOURCES_GLTEXTURECONVERTERS_H__

#include "resources/ResourceLoaderBase.h"

namespace resources 
{
namespace converters 
{
	namespace GLTextureConverters
	{
		/**
		 * Loader class that can convert FreeImage texture resources to OpenGL texture resources.
		 */
		class ConvertFromFI
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

		/**
		 * Loader class that can convert frame buffer resources to OpenGL texture resources.
		 */
		class ConvertFromFB
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

#endif //__RESOURCES_GLTEXTURECONVERTERS_H__
