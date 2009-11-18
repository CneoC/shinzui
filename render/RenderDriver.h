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
// RenderDriver.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_RENDERDRIVER_H__
#define __RENDER_RENDERDRIVER_H__

#include "RendererLoaderBase.h"

namespace os
{
	class ContextBase;
}

namespace render
{
	class RenderDriver
		: public RendererLoaderBase
	{
	protected:
		//! Don't allow external construction.
		RenderDriver(core::Core *pCore)
			: RendererLoaderBase(pCore)
			, m_pContext(NULL)
			, m_pLoaderContext(NULL)
		{}

	public:
		//! Gets the context.
		os::ContextBase *getContext() const			{ return m_pContext; }
		//! Gets the context for the loader thread(s).
		os::ContextBase *getLoaderContext() const	{ return m_pLoaderContext; }

	protected:
		os::ContextBase *m_pContext;
		os::ContextBase *m_pLoaderContext;
	};
}

#endif //__RENDER_RENDERDRIVER_H__
