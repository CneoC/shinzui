#include "GLEndRenderer.h"

#include "os/common//ContextBase.h"

using namespace render;

GLEndRenderer::GLEndRenderer(core::Core *pCore)
	: EndRenderer(pCore)
{
}

GLEndRenderer::~GLEndRenderer()
{
}

void GLEndRenderer::render(double delta)
{
	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);
	getContext()->swapBuffers();
}
