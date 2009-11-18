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
// GLFontResource.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCES_GLFONTRESOURCE_H__
#define __RESOURCES_GLFONTRESOURCE_H__

#include "FontResource.h"

#include <gl/glew.h>

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class GLFontData
		: public FontData
	{
	public:
		static const char *getName()	{ return "GLFont"; }

		//////////////////////////////////////////////////////////////////////////

		GLFontData(ResourceLoaderBase *pData)
			: FontData(pData)
		{
			getType() += getName();
			m_flags[FLAG_REQUIRE_CONTEXT] = true;
		}

		//////////////////////////////////////////////////////////////////////////

		void setTextures(GLuint *pTextures)	{ m_pTextures = pTextures; }
		GLuint *getTextures()				{ return m_pTextures; }
		GLuint getTexture(char c)			{ return m_pTextures[c]; }

		void setDisplayLists(GLuint lists)	{  m_displayLists = lists; }
		GLuint getDisplayLists() const		{ return m_displayLists; }

	protected:
		GLuint *m_pTextures;
		GLuint	m_displayLists;

	};

	typedef ResourceRef<GLFontData>		GLFontResource;
}

#endif //__RESOURCES_GLFONTRESOURCE_H__
