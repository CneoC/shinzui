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
// RenderChain.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_RENDERCHAIN_H__
#define __RENDER_RENDERCHAIN_H__

#include "Renderer.h"

namespace render
{
	/**
	 * Render chain class for rendering multiple renderers in succession.
	 */
	class RenderChain
		: public Renderer
	{
	public:
		typedef std::list<Renderer *>	RendererList;

		//////////////////////////////////////////////////////////////////////////

		RenderChain(core::Core *pCore)
			: Renderer(pCore)
		{}

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Links a renderer to the chain.
		 * @param pRenderer	The renderer to link.
		 */
		RenderChain *link(Renderer *pRenderer)
		{
			m_renderers.push_back(pRenderer);
			return this;
		}

		//! Clears the render chain.
		void clear()
		{
			m_renderers.clear();
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Initializes the entire chain of renderers.
		 */
		virtual void init()
		{
			RendererList::const_iterator renderer;
			for (renderer = m_renderers.begin(); renderer != m_renderers.end(); ++renderer)
			{
				(*renderer)->init();
			}
		}

		/**
		 * Renders the entire chain of renderers.
		 */
		void render(double delta)
		{
			RendererList::const_iterator renderer;
			for (renderer = m_renderers.begin(); renderer != m_renderers.end(); ++renderer)
			{
				(*renderer)->render(delta);
			}
		}

	protected:
		RendererList	m_renderers;
	};
}

#endif //__RENDER_RENDERCHAIN_H__
