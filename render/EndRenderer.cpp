#include "EndRenderer.h"

#include "core/ContextBase.h"

#include <gl/glew.h>

EndRenderer::EndRenderer(Core *pCore, ContextBase *pContext)
	: Renderer(pCore)
	, m_pContext(pContext)
{
}

EndRenderer::~EndRenderer()
{
}

Process *EndRenderer::run(double delta)
{
	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);
	m_pContext->swapBuffers();
	return Renderer::run(delta);
}
