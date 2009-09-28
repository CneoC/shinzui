#include "GLStartRenderer.h"

#include <gl/glew.h>

GLStartRenderer::GLStartRenderer(Core *pCore, ContextBase *pContext)
	: Renderer(pCore)
	, m_pContext(pContext)
{
}

GLStartRenderer::~GLStartRenderer()
{
}

Process *GLStartRenderer::run(double delta)
{
	//if (delta == 0)
		m_pContext->bind();

	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

	return Renderer::run(delta);
}
