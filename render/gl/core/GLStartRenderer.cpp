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
// GLStartRenderer.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "GLStartRenderer.h"

#include "render/RenderDriver.h"

#include <gl/glew.h>

using namespace render;

GLStartRenderer::GLStartRenderer(core::Core *pCore)
	: StartRenderer(pCore)
{
}

GLStartRenderer::~GLStartRenderer()
{
}

void GLStartRenderer::init()
{
	m_pCore->getDriver()->getContext()->bind();
}

void GLStartRenderer::render(double delta)
{
	m_pCore->getDriver()->getContext()->update();

	// Clear The Screen And The Depth Buffer
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
