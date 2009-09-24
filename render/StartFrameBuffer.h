#pragma once

#ifndef __START_FRAME_BUFFER_H__
#define __START_FRAME_BUFFER_H__

#include "Renderer.h"

#include <gl/glew.h>

class StartFrameBuffer
	: public Renderer
{
public:
	StartFrameBuffer(Core *pCore);
	~StartFrameBuffer();

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

#endif //__START_FRAME_BUFFER_H__