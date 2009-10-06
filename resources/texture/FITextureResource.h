#pragma once

#ifndef __RESOURCES_FITEXTURERESOURCE_H__
#define __RESOURCES_FITEXTURERESOURCE_H__

#include "TextureResource.h"

#include <FreeImagePlus.h>

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class FITextureData
		: public TextureData
	{
	public:
		static const char *getName()	{ return "FITexture"; }

		//////////////////////////////////////////////////////////////////////////

		FITextureData(ResourceLoaderBase *pData)
			: TextureData(pData)
		{
			getType() += getName();
		}

		//////////////////////////////////////////////////////////////////////////

		fipImage &getTexture()	{ return m_texture; }

	protected:
		fipImage	m_texture;
	};

	typedef ResourceRef<FITextureData>		FITextureResource;
}

#endif //__RESOURCES_FITEXTURERESOURCE_H__
