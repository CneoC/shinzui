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
		RendererProc(core::Core *pCore, int id = 0)
			: core::Process(pCore, id)
			, m_pRenderer(NULL)
		{
			m_color = math::Color3f(1, 0, 0);
		}

	public:
		//////////////////////////////////////////////////////////////////////////

		virtual void onStart()
		{
			if (getLastRunTime() == 0)
				m_pCore->addJob(this, core::Job::Function(this, &RendererProc::initJob), core::Core::THREAD_ID_MAIN_BIT);
			else
				m_pCore->addJob(this, core::Job::Function(this, &RendererProc::renderJob), core::Core::THREAD_ID_MAIN_BIT);
		}
		
		bool initJob()
		{
			if (m_pRenderer)
				m_pRenderer->init();
			return false;
		}

		bool renderJob()
		{
			if (m_pRenderer)
				m_pRenderer->render(getDeltaTime());
			return false;
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
