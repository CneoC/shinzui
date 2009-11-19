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
// GLContext.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __OS_GLCONTEXT_H__
#define __OS_GLCONTEXT_H__

#include "os/common//ContextBase.h"
#include "os/current/Window.h"

namespace os
{
	class GLContext :
		public ContextBase
	{
	public:
		GLContext(Window *pWindow)
			: ContextBase(pWindow)
			, m_hRC(NULL)
		{
		}
		
		~GLContext()
		{
			assert(!m_hRC);
		}

		virtual bool create(Type type);
		virtual bool destroy();

		virtual bool bind();
		virtual bool unbind();
		virtual bool link(ContextBase *pOther);

		virtual void update();

		virtual void swapBuffers();
		
		//////////////////////////////////////////////////////////////////////////

		HGLRC getHRC() const	{ return m_hRC; }

	protected:
		bool init();
		bool resize(const math::Vector2i &size);

	protected:
		HGLRC		m_hRC;		// OpenGL rendering context
	};
}

#endif //__OS_GLCONTEXT_H__
