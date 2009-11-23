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
// GLRenderDriver.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_GLRENDERDRIVER_H__
#define __RENDER_GLRENDERDRIVER_H__

#include "render/RenderDriver.h"

#include "os/current/Window.h"
#include "os/current/gl/GLContext.h"

namespace render
{
	class GLRenderDriver
		: public RenderDriver
	{
	public:
		//! 
		GLRenderDriver(core::Core *pCore, os::Window *pWindow);
		~GLRenderDriver();

		//! Get window.
		os::Window *getWindow() const				{ return m_pWindow; }

		//! Get context.
		os::GLContext *getGLContext() const			{ return static_cast<os::GLContext *>(m_pContext); }
		os::GLContext *getGLLoaderContext() const	{ return static_cast<os::GLContext *>(m_pLoaderContext); }

	protected:
		void setPixelFormat();
		void init();

		os::Window *	m_pWindow;
	};
}

#endif __RENDER_GLRENDERDRIVER_H__
