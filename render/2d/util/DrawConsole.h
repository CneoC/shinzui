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
// Console.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_DRAWCONSOLE_H__
#define __RENDER_DRAWCONSOLE_H__

#include "render/Renderer.h"
#include "render/2d/FontUtil.h"

#include "resources/font/FontResource.h"

namespace console
{
	class Console;
}

namespace render
{
	class DrawConsole
		: public render::Renderer
	{
	public:
		DrawConsole(core::Core *pCore, const console::Console &console);
		~DrawConsole();

		virtual void render(double delta);

	protected:
		const console::Console &	m_console;

		render::FontUtil *			m_pFontUtil;
		resources::FontResource		m_font;
	};
}

#endif //__RENDER_DRAWCONSOLE_H__
