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
// StartFrameBuffer.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_STARTFRAMEBUFFER_H__
#define __RENDER_STARTFRAMEBUFFER_H__

#include "render/Renderer.h"

#include "resources/texture/FrameBufferResource.h"

//////////////////////////////////////////////////////////////////////////

namespace render
{
	class StartFrameBuffer
		: public Renderer
	{
	public:
		StartFrameBuffer(core::Core *pCore) : Renderer(pCore) {}

		//////////////////////////////////////////////////////////////////////////

		void setFrameBuffer(resources::FrameBufferResource frameBuffer)	{ m_frameBuffer = frameBuffer; }
		resources::FrameBufferResource getFrameBuffer()					{ return m_frameBuffer; }

	protected:
		resources::FrameBufferResource	m_frameBuffer;
	};
}

#endif //__RENDER_STARTFRAMEBUFFER_H__
