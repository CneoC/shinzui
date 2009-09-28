#pragma once

#ifndef __RENDER_GLSTARTRENDERER_H__
#define __RENDER_GLSTARTRENDERER_H__

#include "render/Renderer.h"

#include "os/interface/ContextBase.h"

class GLStartRenderer :
	public Renderer
{
public:
	GLStartRenderer(Core *pCore, ContextBase *pContext);
	~GLStartRenderer();

	virtual Process *run(double delta);

protected:
	ContextBase *m_pContext;
};

#endif //__RENDER_GLSTARTRENDERER_H__
