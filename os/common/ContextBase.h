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
// ContextBase.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __OS_CONTEXTBASE_H__
#define __OS_CONTEXTBASE_H__

#include "os/current/Window.h"

namespace os
{
	/**
	 * Base class for rendering contexts.
	 */
	class ContextBase
	{
	public:
		enum Flags
		{
			FLAG_RESIZED
		};

		enum Type
		{
			CONTEXT_NONE,
			CONTEXT_RENDER,
			CONTEXT_LOAD
		};

		//////////////////////////////////////////////////////////////////////////

		ContextBase(Window *pWindow)
			: m_pWindow(pWindow)
			, m_type(CONTEXT_NONE)
		{
			pWindow->resizeEvent.connect(this, &ContextBase::onResize);
		}

		//////////////////////////////////////////////////////////////////////////

		//! Creates the context.
		virtual bool create(Type type) = 0;
		//! Destroys the context.
		virtual bool destroy() = 0;

		//! Binds the context to the current thread.
		virtual bool bind() = 0;
		//! Unbinds the context from the current thread.
		virtual bool unbind() = 0;

		//! Links the resources of 2 contexts.
		virtual bool link(ContextBase *pOther) = 0;

		//! Updates the context.
		virtual void update() = 0;

		//! Swap the buffers for this context.
		virtual void swapBuffers() = 0;

		//////////////////////////////////////////////////////////////////////////

		//! Gets the window this context belongs to.
		Window *getWindow() const	{ return m_pWindow; }

	protected:
		//! Resize event
		void onResize(const math::Vector2i &size) { m_flags[FLAG_RESIZED] = true; }

	protected:
		std::bitset<16>	m_flags;
		u16				m_type;
		Window *		m_pWindow;
	};
}

#endif //__OS_CONTEXTBASE_H__
