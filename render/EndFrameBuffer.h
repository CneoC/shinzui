#pragma once

#ifndef __END_FRAME_BUFFER_H__
#define __END_FRAME_BUFFER_H__

#include "Renderer.h"

class EndFrameBuffer
	: public Renderer
{
public:
	EndFrameBuffer(Core *pCore);
	~EndFrameBuffer();

	virtual Process *run(double delta);

protected:
};

#endif //__END_FRAME_BUFFER_H__
