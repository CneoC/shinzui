#pragma once

#ifndef __RENDER_GLSTARTFRAMEBUFFER_H__
#define __RENDER_GLSTARTFRAMEBUFFER_H__

#include "render/core/StartFrameBuffer.h"

#include <gl/glew.h>

namespace render
{
	class GLStartFrameBuffer
		: public StartFrameBuffer
	{
	public:
		static Renderer *create(Core *pCore)	{ return new GLStartFrameBuffer(pCore); }

		//////////////////////////////////////////////////////////////////////////

		GLStartFrameBuffer(Core *pCore);
		~GLStartFrameBuffer();

		virtual void render(double delta);
	};
}

#endif //__RENDER_GLSTARTFRAMEBUFFER_H__
