#pragma once

#ifndef __RENDER_RENDERER_H__
#define __RENDER_RENDERER_H__

#include "core/Process.h"

namespace render
{
	/**
	 * Render process calling a renderer on run.
	 */
	class RendererProc
		: public Process
	{
	public:
		RendererProc(Core *pCore, int id = 0, int targetThreadId = Core::THREAD_ID_MAIN_BIT)
			: m_pRenderer(NULL)
		{}

	public:
		virtual Process *run(double delta)
		{
			if (m_pRenderer)
				m_pRenderer->render(delta);
		}

		//! Sets the active renderer.
		void setRenderer(Renderer *pRenderer)	{ m_pRenderer = pRenderer; }
		//! Gets the active renderer.
		Renderer *getRenderer() const			{ return m_pRenderer; }

	protected:
		Renderer *m_pRenderer;
	}
}

#endif __RENDER_RENDERER_H__
