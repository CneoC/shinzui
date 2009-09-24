#include "EndRenderer.h"

#include <gl/glew.h>

EndRenderer::EndRenderer(Core *pCore)
	: Renderer(pCore)
{
}

EndRenderer::~EndRenderer()
{
}

Process *EndRenderer::run(double delta)
{
	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);
	return Renderer::run(delta);
}
