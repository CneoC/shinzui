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

#include "GLFontUtil.h"

#include "resources/ResourceLoaderBase.h"

#include <stdexcept>

using namespace render;
using namespace resources;

GLFontUtil::GLFontUtil(GLFontResource font)
	: FontUtil(font)
{
}

void GLFontUtil::print(const math::Vector2f &p, size_t length, const char *text) const
{
	GLFontResource font(m_font);
	if (!font)
		throw std::runtime_error("Invalid font resource, expected GLFontResource.");

	// Set proper render settings for font rendering.
	glPushAttrib(GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_ENABLE_BIT | GL_LIST_BIT); 
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(p.x, p.y, 0);

	// Call the display lists for each character in the formatted text
	glListBase(font->getDisplayLists());
	glCallLists(length, GL_UNSIGNED_BYTE, text);

	glPopMatrix();
	glPopAttrib();
}
