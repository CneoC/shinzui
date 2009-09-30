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
		/**
		 * Links a renderer to the chain.
		 * @param pRenderer	The renderer to link.
		 */
		RenderChain *link(Renderer *pRenderer)
		{
			if (!m_pFirst)
				m_pFirst = pRenderer;
			m_pLast->setNext(pRenderer);
			m_pLast = pRenderer;
			return *this;
		}

		/**
		 * Renders the entire chain of renderers.
		 */
		void render(double delta)
		{
			Renderer *pRenderer = m_pFirst;
			while (pRenderer)
			{
				pRenderer->render(delta);
				pRenderer = pRenderer->getNext();
			}
		}

	protected:
		Renderer *m_pFirst;	// first renderer in the chain
		Renderer *m_pLast;	// last renderer in the chain
	};
}

#endif //__RENDER_RENDERCHAIN_H__
