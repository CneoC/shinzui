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
// Console.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "DrawConsole.h"

#include "resources/ResourceLoader.h"
#include "resources/ResourceCache.h"
#include "resources/font/GLFontResource.h"
#include "resources/font/FTFontResource.h"

#include "util/console/Console.h"

#include "os/current/gl/GLContext.h"

using namespace render;
using namespace resources;
using namespace console;

//////////////////////////////////////////////////////////////////////////

DrawConsole::DrawConsole(core::Core *pCore, const Console &console)
	: Renderer(pCore)
	, m_console(console)
{
	m_pFontUtil = m_pCore->getDriver()->getUtil("2d.Font")->as<render::FontUtil>();

	FTFontResource ftFont(m_pCore->getLoader(), "File::2d/fonts/debug.ttf", "console");
	ftFont->setSize(16, 16);

	m_font = GLFontResource(ftFont);
	m_font.load();
}

DrawConsole::~DrawConsole()
{
}

void DrawConsole::render(double delta)
{
	m_pFontUtil->setResource(m_font);

	glPushAttrib(GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_ENABLE_BIT); 
	glDisable(GL_DEPTH_TEST);

	GLint rec[4];
	glGetIntegerv(GL_VIEWPORT, rec);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, rec[2], rec[3], 0.0f, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);
	glLoadIdentity();

	math::Vector2f pos(10, rec[3] - 50);
	Console::ConsoleBuffer::const_reverse_iterator lineIter = m_console.getBuffer().rbegin();
	while (lineIter != m_console.getBuffer().rend() && pos.y > 0)
	{
		const ConsoleLine &line = *lineIter;

		m_pFontUtil->print(pos, line.getText().length(), line.getText().c_str());
		pos.y -= 14;

		++lineIter;
	}

	glPopAttrib();
}
