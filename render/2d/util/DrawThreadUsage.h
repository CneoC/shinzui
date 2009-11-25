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
// DrawThreadUsage.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_DRAWTHREADUSAGE_H__
#define __RENDER_DRAWTHREADUSAGE_H__

#include "render/Renderer.h"

#include "core/Core.h"
#include "core/Process.h"
#include "os/current/Time.h"

namespace console
{
	class ThreadUsage;
}

namespace render
{
	class DrawThreadUsage
		: public render::Renderer
	{
	public:
		DrawThreadUsage(core::Core *pCore, const console::ThreadUsage &usage, double showDuration = 2);
		~DrawThreadUsage();

		virtual void render(double delta);

	protected:
		const console::ThreadUsage &	m_usage;
		double							m_showDuration;
	};
}

#endif //__RENDER_DRAWTHREADUSAGE_H__
