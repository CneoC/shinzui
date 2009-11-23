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
// GLFontUtil.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_GLFONTUTIL_H__
#define __RENDER_GLFONTUTIL_H__

#include "render/2d/FontUtil.h"

#include "math/Math.h"
#include "math/Vector2.h"

#include "resources/ResourceLoaderBase.h"
#include "resources/font/GLFontResource.h"

#pragma warning(disable: 4996)

namespace render
{
	class GLFontUtil
		: public FontUtil
	{
	public:
		GLFontUtil(resources::GLFontResource font = resources::Resource());

		/**
		 * Prints a single line of text.
		 * @param p			Translation to render the text at.
		 * @param length	Length of the text to render.
		 * @param text		Text to render.
		 */
		virtual void print(const math::Vector2f &p, size_t length, const char *text) const;
	};
}

#endif //__RENDER_GLFONTUTIL_H__
