#include "Renderer.h"

#include <gl/glew.h>

Renderer::Renderer(Core *pCore, int id, int targetThread)
	: ChainProcess(pCore, id, targetThread)
{
}

Renderer::~Renderer()
{
}
