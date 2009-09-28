#include "GLEndRenderer.h"

#include "os/interface/ContextBase.h"

#include <gl/glew.h>

GLEndRenderer::GLEndRenderer(Core *pCore, ContextBase *pContext)
	: Renderer(pCore)
	, m_pContext(pContext)
{
}

GLEndRenderer::~GLEndRenderer()
{
}

Process *GLEndRenderer::run(double delta)
{
	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);
	m_pContext->swapBuffers();
	return Renderer::run(delta);
}
