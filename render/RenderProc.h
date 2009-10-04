#pragma once

#ifndef __RENDER_RENDERERPROC_H__
#define __RENDER_RENDERERPROC_H__

#include "core/Process.h"

namespace render
{
	/**
	 * Render process calling a renderer on run.
	 */
	class RendererProc
		: public core::Process
	{
	public:
		RendererProc(core::Core *pCore, int id = 0, int targetThreadId = core::Core::THREAD_ID_MAIN_BIT)
			: core::Process(pCore, id, targetThreadId)
			, m_pRenderer(NULL)
		{}

	public:
		virtual void init()
		{
			if (m_pRenderer)
				m_pRenderer->init();
		}

		virtual core::Process *run(double delta)
		{
			if (m_pRenderer)
				m_pRenderer->render(delta);
			return this;
		}

		//! Sets the active renderer.
		void setRenderer(Renderer *pRenderer)	{ m_pRenderer = pRenderer; }
		//! Gets the active renderer.
		Renderer *getRenderer() const			{ return m_pRenderer; }

	protected:
		Renderer *m_pRenderer;
	};
}

#endif //__RENDER_RENDERERPROC_H__