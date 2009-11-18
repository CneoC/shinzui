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
// FontUtil.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "FontUtil.h"

#include <assert.h>
#include <stdarg.h>

using namespace render;
using namespace resources;

#pragma warning(disable: 4996)

FontUtil::FontUtil(FontResource font)
	: m_font(font)
{
}

void FontUtil::printf(const math::Vector2f &p, const char *fmt, ...) const
{
	assert(fmt);

	// Parse and print variable argument list.
	char	text[512];
	va_list	ap;
	va_start(ap, fmt);
	size_t length = vsprintf(text, fmt, ap);
	va_end(ap);

	print(p, length, text);
}
