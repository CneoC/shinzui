#include "GLStartFrameBuffer.h"

#include <gl/glew.h>

using namespace render;

GLStartFrameBuffer::GLStartFrameBuffer(core::Core *pCore)
	: StartFrameBuffer(pCore)
{
}

GLStartFrameBuffer::~GLStartFrameBuffer()
{
}

void GLStartFrameBuffer::render(double delta)
{
	if (m_frameBuffer)
		m_frameBuffer->bind();
}
