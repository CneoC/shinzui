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
