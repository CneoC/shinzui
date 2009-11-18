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
// GLRenderDriver.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "GLRenderDriver.h"

#include "render/gl/core/GLStartRenderer.h"
#include "render/gl/core/GLEndRenderer.h"
#include "render/gl/core/GLStartFrameBuffer.h"
#include "render/gl/core/GLEndFrameBuffer.h"

#include "render/gl/2d/GL2DLoader.h"

using namespace render;

//////////////////////////////////////////////////////////////////////////

GLRenderDriver::GLRenderDriver(core::Core *pCore, os::Window *pWindow)
	: RenderDriver(pCore)
	, m_pWindow(pWindow)
{
	m_pContext = new os::GLContext(pWindow);
	getGLContext()->create();

	m_pLoaderContext = new os::GLContext(pWindow);
	getLoaderGLContext()->create();
	getLoaderGLContext()->unbind();

	getGLContext()->link(getLoaderGLContext());

	// Register core renderers
	addRenderer("Start",	&GLStartRenderer::create);
	addRenderer("End",		&GLEndRenderer::create);
	addRenderer("StartFB",	&GLStartFrameBuffer::create);
	addRenderer("EndFB",	&GLEndFrameBuffer::create);

	// Register core utilities

	// Register core loaders
	addLoader("2d", new GL2DLoader(pCore));
}

GLRenderDriver::~GLRenderDriver()
{
	getGLContext()->destroy();
	getLoaderGLContext()->destroy();
}
