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
		static Renderer *create(Core *pCore)	{ return new GLEndFrameBuffer(pCore); }

		//////////////////////////////////////////////////////////////////////////

		GLEndFrameBuffer(Core *pCore);
		~GLEndFrameBuffer();

		virtual void render(double delta);
	};
}

#endif //__RENDER_GLENDFRAMEBUFFER_H__
