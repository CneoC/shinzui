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
// GLStartRenderer.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_GLSTARTRENDERER_H__
#define __RENDER_GLSTARTRENDERER_H__

#include "render/core/StartRenderer.h"

#include "os/common//ContextBase.h"

namespace render
{
	class GLStartRenderer :
		public StartRenderer
	{
	public:
		static Renderer *create(core::Core *pCore)	{ return new GLStartRenderer(pCore); }

		//////////////////////////////////////////////////////////////////////////

		GLStartRenderer(core::Core *pCore);
		~GLStartRenderer();

		//////////////////////////////////////////////////////////////////////////

		virtual void init();
		virtual void render(double delta);

	protected:
	};
}

#endif //__RENDER_GLSTARTRENDERER_H__
