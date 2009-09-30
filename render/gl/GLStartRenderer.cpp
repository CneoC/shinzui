#include "GLStartRenderer.h"

#include "render/RenderDriver.h"

#include <gl/glew.h>

using namespace render;

GLStartRenderer::GLStartRenderer(Core *pCore)
	: GLRenderer(pCore)
{
}

GLStartRenderer::~GLStartRenderer()
{
}

void GLStartRenderer::render(double delta)
{
	//if (delta == 0)
	m_pCore->getDriver()->getContext()->bind();

	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);

	// Clear The Screen And The Depth Buffer
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
