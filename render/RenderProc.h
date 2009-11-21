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
// RenderProc.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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
			m_pCore->addJob(this, core::Job::Function(this, &RendererProc::initJob), core::Core::THREAD_ID_RENDER_BIT);
		}

	public:
		//////////////////////////////////////////////////////////////////////////

		virtual void onStart()
		{
			m_pCore->addJob(this, core::Job::Function(this, &RendererProc::renderJob), core::Core::THREAD_ID_RENDER_BIT);
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
