#pragma once

#ifndef __RENDER_GLENDRENDERER_H__
#define __RENDER_GLENDRENDERER_H__

#include "render/Renderer.h"

class ContextBase;

class GLEndRenderer :
	public Renderer
{
public:
	GLEndRenderer(Core *pCore, ContextBase *pContext);
	~GLEndRenderer();

	virtual Process *run(double delta);

protected:
	ContextBase *m_pContext;
};

#endif //__RENDER_GLENDRENDERER_H__
