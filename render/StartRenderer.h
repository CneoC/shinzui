#pragma once

#ifndef __START_RENDERER_H__
#define __START_RENDERER_H__

#include "Renderer.h"

#include "core/ContextBase.h"

class StartRenderer :
	public Renderer
{
public:
	StartRenderer(Core *pCore, ContextBase *pContext);
	~StartRenderer();

	virtual Process *run(double delta);

protected:
	ContextBase *m_pContext;
};

#endif //__START_RENDERER_H__
