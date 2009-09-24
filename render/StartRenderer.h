#pragma once

#ifndef __START_RENDERER_H__
#define __START_RENDERER_H__

#include "Renderer.h"

class StartRenderer :
	public Renderer
{
public:
	StartRenderer(Core *pCore);
	~StartRenderer();

	virtual Process *run(double delta);
};

#endif //__START_RENDERER_H__
