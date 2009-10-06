#pragma once

#ifndef __RESOURCES_GLPROGRAMCONVERTERS_H__
#define __RESOURCES_GLPROGRAMCONVERTERS_H__

#include "resources/ResourceLoaderBase.h"

namespace resources
{
namespace converters
{
	namespace GLProgramConverters
	{
		/**
		 * Loader class that can convert program definition resources to gl program resources.
		 */
		class ConvertFromProgram
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

#endif //__RESOURCES_GLPROGRAMCONVERTERS_H__
