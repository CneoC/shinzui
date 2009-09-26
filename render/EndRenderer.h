#pragma once

#ifndef __END_RENDERER_H__
#define __END_RENDERER_H__

#include "Renderer.h"

class ContextBase;

class EndRenderer :
	public Renderer
{
public:
	EndRenderer(Core *pCore, ContextBase *pContext);
	~EndRenderer();

	virtual Process *run(double delta);

protected:
	ContextBase *m_pContext;
};

#endif //__END_RENDERER_H__
