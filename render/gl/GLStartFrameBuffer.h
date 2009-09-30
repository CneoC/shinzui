#pragma once

#ifndef __RENDER_GLSTARTFRAMEBUFFER_H__
#define __RENDER_GLSTARTFRAMEBUFFER_H__

#include "GLRenderer.h"

#include <gl/glew.h>

namespace render
{
	class GLStartFrameBuffer
		: public GLRenderer
	{
	public:
		GLStartFrameBuffer(Core *pCore);
		~GLStartFrameBuffer();

		virtual void render(double delta);

	protected: 
		GLsizei width;
		GLsizei height;
		//struct PHviewportRec viewport;
		GLfloat clearColor[4];
		GLfloat modelview[16];
		GLfloat projection[16];
		GLuint texture;
		GLuint depth;
		GLuint fbo;
	};
}

#endif //__RENDER_GLSTARTFRAMEBUFFER_H__
