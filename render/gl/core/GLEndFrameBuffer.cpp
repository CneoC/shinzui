#include "GLEndFrameBuffer.h"

#include "resources/ResourceLoaderBase.h"

#include <gl/glew.h>

#include <math.h>

using namespace render;

GLEndFrameBuffer::GLEndFrameBuffer(Core *pCore)
	: EndFrameBuffer(pCore)
{
}

GLEndFrameBuffer::~GLEndFrameBuffer()
{
}

void GLEndFrameBuffer::render(double delta)
{
	if (m_frameBuffer)
		m_frameBuffer->unbind();

	GLint viewportRect[4];
	glGetIntegerv(GL_VIEWPORT, viewportRect);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (m_frameBuffer->getTexture().isLoaded())
	{
		glEnable(GL_TEXTURE_2D);
		m_frameBuffer->getTexture()->bind();
	}

	if (m_program.isLoaded())
		m_program->bind();

	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2f(0, 0);
		glTexCoord2i(1, 0); glVertex2f(1, 0);
		glTexCoord2i(1, 1); glVertex2f(1, 1);
		glTexCoord2i(0, 1); glVertex2f(0, 1);
	glEnd();

	if (m_program.isLoaded())
		m_program->unbind();

	if (m_frameBuffer->getTexture().isLoaded())
	{
		m_frameBuffer->getTexture()->unbind();
		glDisable(GL_TEXTURE_2D);
	}

	glPopMatrix();
}
