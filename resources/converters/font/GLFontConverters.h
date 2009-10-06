#pragma once

#ifndef __RESOURCES_GLFONTCONVERTERS_H__
#define __RESOURCES_GLFONTCONVERTERS_H__

#include "resources/ResourceLoaderBase.h"

#include "resources/font/GLFontResource.h"

namespace resources
{
namespace converters
{
	namespace GLFontConverters
	{
		/**
		 * Loader class that can convert FreeType2 font resources to OpenGL font resources.
		 */
		class ConvertFromFT
			: public ResourceLoaderBase
		{
		public:
			//! @see ResourceLoaderBase::convert
			virtual Resource convert(const Resource &res, const ResourceType &type);

			//! @see ResourceLoaderBase::load
			virtual bool load(Resource &res, u32 flags);

			//! @see ResourceLoaderBase::unload
			virtual bool unload(Resource &res, u32 flags);

		protected:
			/**
			 * Creates a display list entry for a font resource character.
			 * @param font	The font to create the display list for.
			 * @param c		The character to generate.
			 */
			void createDisplayList(GLFontResource &font, unsigned char c);
		};
	}
}
}

#endif //__RESOURCES_GLFONTCONVERTERS_H__
