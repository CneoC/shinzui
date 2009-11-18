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
// FontUtil.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_FONTUTIL_H__
#define __RENDER_FONTUTIL_H__

#include "render/RenderUtil.h"

#include "math/Math.h"
#include "math/Vector2.h"

#include "resources/font/FontResource.h"

#include <gl/glew.h>

namespace render
{
	class FontUtil
		: public RenderUtil
	{
	public:
		FontUtil(resources::FontResource font = resources::Resource());

		void setResource(resources::FontResource font) { m_font = font; }

		/**
		 * Prints a single line of text.
		 * @param p			Translation to render the text at.
		 * @param length	Length of the text to render.
		 * @param text		Text to render.
		 */
		virtual void print(const math::Vector2f &p, size_t length, const char *text) const = 0;

		/**
		 * Prints a single line of formatted text.
		 * @param p		translation to render the text at.
		 * @param fmt	format to print using variable argument list.
		 */
		void printf(const math::Vector2f &p, const char *fmt, ...) const;

	protected:
		resources::FontResource m_font;
	};
}


#endif //__RENDER_FONTUTIL_H__
