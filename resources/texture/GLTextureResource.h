//////////////////////////////////////////////////////////////////////////
//
// This file is part of Shinzui.
//
// Shinzui is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shinzui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shinzui.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////
//
// GLTextureResource.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCES_GLTEXTURERESOURCE_H__
#define __RESOURCES_GLTEXTURERESOURCE_H__

#include "TextureResource.h"

#include "os/current/gl/GLContext.h"

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
