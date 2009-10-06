#pragma once

#ifndef __RESOURCES_GLTEXTURERESOURCE_H__
#define __RESOURCES_GLTEXTURERESOURCE_H__

#include "TextureResource.h"

#include <gl/glew.h>

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class GLTextureData
		: public TextureData
	{
	public:
		static const char *getName()	{ return "GLTexture"; }

		//////////////////////////////////////////////////////////////////////////

		GLTextureData(ResourceLoaderBase *pData)
			: TextureData(pData)
			, m_texture(GL_NONE)
		{
			getType() += getName();
			m_flags[FLAG_REQUIRE_CONTEXT] = true;
		}

		//////////////////////////////////////////////////////////////////////////

		void setTexture(GLuint texture)	{ m_texture = texture; }
		GLuint getTexture()				{ return m_texture; }

		virtual void bind()
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		virtual void unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	protected:
		GLuint m_texture;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef ResourceRef<GLTextureData>		GLTextureResource;
}

#endif //__RESOURCES_GLTEXTURERESOURCE_H__
