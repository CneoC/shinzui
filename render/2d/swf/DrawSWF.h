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
// DrawFPS.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_DRAWSWF_H__
#define __RENDER_DRAWSWF_H__

#include "render/Renderer.h"
#include "render/2d/FontUtil.h"

#include "resources/font/FontResource.h"

#include "gameswf/gameswf.h"

namespace gameswf
{
	struct player;
	struct render_handler;
}

namespace render
{
	class DrawSWF
		: public render::Renderer
	{
	public:
		DrawSWF(core::Core *pCore);
		~DrawSWF();

		virtual void render(double delta);

	protected:
		gameswf::gc_ptr<gameswf::player> m_pPlayer;
		gameswf::gc_ptr<gameswf::root> m_pRoot;
		gameswf::render_handler* m_pRenderer;
	};
}

#endif //__RENDER_DRAWFPS_H__
