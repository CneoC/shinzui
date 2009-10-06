#pragma once

#ifndef __RESOURCES_GLFRAMEBUFFERCONVERTERS_H__
#define __RESOURCES_GLFRAMEBUFFERCONVERTERS_H__

#include "resources/ResourceLoaderBase.h"

namespace resources
{
namespace converters
{
	namespace GLFrameBufferConverters
	{
		/**
		* Loader class that can convert framebuffer resources to OpenGL framebuffer resources.
		*/
		class ConvertFromDef
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

#endif //__RESOURCES_GLFRAMEBUFFERCONVERTERS_H__
