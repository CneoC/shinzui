#pragma once

#ifndef __RENDER_GLENDFRAMEBUFFER_H__
#define __RENDER_GLENDFRAMEBUFFER_H__

#include "render/core/EndFrameBuffer.h"

#include "resources/shader/GLProgramResource.h"

namespace render
{
	class GLEndFrameBuffer
		: public EndFrameBuffer
	{
	public:
		static Renderer *create(core::Core *pCore)	{ return new GLEndFrameBuffer(pCore); }

		//////////////////////////////////////////////////////////////////////////

		GLEndFrameBuffer(core::Core *pCore);
		~GLEndFrameBuffer();

		virtual void render(double delta);
	};
}

#endif //__RENDER_GLENDFRAMEBUFFER_H__
