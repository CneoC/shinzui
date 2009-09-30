#include "GLEndRenderer.h"

#include "os/interface/ContextBase.h"

#include <gl/glew.h>

using namespace render;

GLEndRenderer::GLEndRenderer(Core *pCore)
	: GLRenderer(pCore)
{
}

GLEndRenderer::~GLEndRenderer()
{
}

void GLEndRenderer::render(double delta)
{
	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);
	getGLContext()->swapBuffers();
}
