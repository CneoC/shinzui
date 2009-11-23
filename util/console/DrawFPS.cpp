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
// DrawFPS.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "DrawFPS.h"

#include "core/Core.h"

#include "render/RenderDriver.h"
#include "render/2d/FontUtil.h"

#include "resources/ResourceLoader.h"
#include "resources/ResourceCache.h"
#include "resources/font/GLFontResource.h"
#include "resources/font/FTFontResource.h"

#include "os/current/gl/GLContext.h"

using namespace console;
using namespace resources;

//////////////////////////////////////////////////////////////////////////

DrawFPS::DrawFPS(core::Core *pCore)
	: render::Renderer(pCore)
	, m_frameCount(0)
	, m_frameTime(0)
	, m_fps(0)
{
	m_pFontUtil = m_pCore->getDriver()->getUtil("2d.Font")->as<render::FontUtil>();

	FTFontResource ftFont(m_pCore->getLoader(), "File::2d/fonts/debug.ttf", "fps");
	ftFont->setSize(14, 96);

	m_font = GLFontResource(ftFont);
	m_font.load();
}

DrawFPS::~DrawFPS()
{
}

void DrawFPS::render(double delta)
{
	m_pFontUtil->setResource(m_font);

	m_frameCount++;
	m_frameTime += delta;
	if (m_frameTime > 0.5)
	{
		m_fps = m_frameCount / m_frameTime;
		m_frameTime = 0;
		m_frameCount = 0;
	}

	glPushAttrib(GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_ENABLE_BIT); 
	glDisable(GL_DEPTH_TEST);

	GLint rec[4];
	glGetIntegerv(GL_VIEWPORT, rec);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, rec[2], rec[3], 0.0f, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	ResourceCache *pCache = m_pCore->getLoader()->getCache();
	if (pCache->isLoading())
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		m_pFontUtil->printf(math::Vector2f(getContext()->getWindow()->getSize().x - 150.0f, 10.0f), "Loading... %3.1f%%", pCache->getProgress() * 100);
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	glLoadIdentity();
	m_pFontUtil->printf(math::Vector2f(10, 10), "%04.2f FPS", getFPS());

	glPopAttrib();
}