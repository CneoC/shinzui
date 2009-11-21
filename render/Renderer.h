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
// Renderer.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_RENDERER_H__
#define __RENDER_RENDERER_H__

#include "core/types.h"

#include "render/RenderDriver.h"
#include "os/common/ContextBase.h"

namespace render
{
	/**
	 * Base class for a renderer.
	 */
	class Renderer
	{
	protected:
		//! Don't allow external construction.
		Renderer(core::Core *pCore)
			: m_pCore(pCore)
		{}

	public:
		/**
		 * Initialize renderer.
		 */
		virtual void init() {}

		/**
		 * Renders this renderer.
		 * @param delta	time delta since last render
		 */
		virtual void render(double delta) = 0;

		/**
		 * Cast to different renderer.
		 */
		template <class T>
		T *as()	{ return static_cast<T *>(this); }

	protected:
		//! Get context.
		os::ContextBase *getContext() const			{ return m_pCore->getDriver()->getContext(); }
		os::ContextBase *getLoaderContext() const	{ return m_pCore->getDriver()->getLoaderContext(); }

	protected:
		core::Core *		m_pCore;	// core class.
	};
}

#endif //__RENDER_RENDERER_H__
