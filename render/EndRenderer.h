#pragma once

#ifndef __END_RENDERER_H__
#define __END_RENDERER_H__

#include "Renderer.h"

class EndRenderer :
	public Renderer
{
public:
	EndRenderer(Core *pCore);
	~EndRenderer();

	virtual Process *run(double delta);
};

#endif //__END_RENDERER_H__
