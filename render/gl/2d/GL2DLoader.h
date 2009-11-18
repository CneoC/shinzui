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
// GL2DLoader.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_GL2DLOADER_H__
#define __RENDER_GL2DLOADER_H__

#include "render/RendererLoaderBase.h"

#include "GLFontUtil.h"

namespace render
{
	class GL2DLoader
		: public RendererLoaderBase
	{
	public:
		GL2DLoader(core::Core *pCore)
			: RendererLoaderBase(pCore)
		{
			addUtil("Font", new GLFontUtil());
		}
	};
}

#endif __RENDER_GL2DLOADER_H__
