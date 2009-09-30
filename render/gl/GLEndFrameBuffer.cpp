#include "GLEndFrameBuffer.h"

#include <gl/glew.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace render;

GLEndFrameBuffer::GLEndFrameBuffer(Core *pCore)
	: GLRenderer(pCore)
{
}

GLEndFrameBuffer::~GLEndFrameBuffer()
{
}

void GLEndFrameBuffer::render(double delta)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	GLint viewportRect[4];
	glGetIntegerv(GL_VIEWPORT, viewportRect);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(	viewportRect[0], viewportRect[2],
				viewportRect[1], viewportRect[3]);

	if (m_program) glUseProgram(m_program->getProgram());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 1);

	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(0, 0);
		glTexCoord2i(1, 0); glVertex2i(500, 0);
		glTexCoord2i(1, 1); glVertex2i(500, 500);
		glTexCoord2i(0, 1); glVertex2i(0, 500);
	glEnd();

	if (m_program) glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}
