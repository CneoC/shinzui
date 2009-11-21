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
// GLStartFrameBuffer.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_GLSTARTFRAMEBUFFER_H__
#define __RENDER_GLSTARTFRAMEBUFFER_H__

#include "render/core/StartFrameBuffer.h"

namespace render
{
	class GLStartFrameBuffer
		: public StartFrameBuffer
	{
	public:
		static Renderer *create(core::Core *pCore)	{ return new GLStartFrameBuffer(pCore); }

		//////////////////////////////////////////////////////////////////////////

		GLStartFrameBuffer(core::Core *pCore);
		~GLStartFrameBuffer();

		virtual void render(double delta);
	};
}

#endif //__RENDER_GLSTARTFRAMEBUFFER_H__
