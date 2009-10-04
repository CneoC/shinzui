#include "GLStartRenderer.h"

#include "render/RenderDriver.h"

#include <gl/glew.h>

using namespace render;

GLStartRenderer::GLStartRenderer(core::Core *pCore)
	: StartRenderer(pCore)
{
}

GLStartRenderer::~GLStartRenderer()
{
}

void GLStartRenderer::init()
{
	m_pCore->getDriver()->getContext()->bind();
}

void GLStartRenderer::render(double delta)
{
	m_pCore->getDriver()->getContext()->update();

	// Clear The Screen And The Depth Buffer
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
