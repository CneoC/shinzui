#pragma once

#ifndef __RENDER_GLSTARTFRAMEBUFFER_H__
#define __RENDER_GLSTARTFRAMEBUFFER_H__

#include "render/Renderer.h"

#include <gl/glew.h>

class GLStartFrameBuffer
	: public Renderer
{
public:
	GLStartFrameBuffer(Core *pCore);
	~GLStartFrameBuffer();

	virtual Process *run(double delta);

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

#endif //__RENDER_GLSTARTFRAMEBUFFER_H__
