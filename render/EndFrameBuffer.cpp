#include "EndFrameBuffer.h"

#include <gl/glew.h>

#define _USE_MATH_DEFINES
#include <math.h>

EndFrameBuffer::EndFrameBuffer(Core *pCore)
	: Renderer(pCore)
{
}

EndFrameBuffer::~EndFrameBuffer()
{
}

GLuint phCompile(const char *vert, const char *frag)
{
	GLchar buf[256];
	GLuint vertShader, fragShader, program;
	GLint success;

	vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, (const GLchar**) &vert, 0);
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertShader, sizeof(buf), 0, buf);
		//debugf(buf);
		//fatalf("Unable to compile vertex shader.\n");
	}

	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, (const GLchar**) &frag, 0);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShader, sizeof(buf), 0, buf);
		//debugf(buf);
		//fatalf("Unable to compile fragment shader.\n");
	}

	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, sizeof(buf), 0, buf);
		//debugf(buf);
		//fatalf("Unable to link shaders.\n");
	}

	return program;
}

Process *EndFrameBuffer::run(double delta)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	GLint viewportRect[4];
	glGetIntegerv(GL_VIEWPORT, viewportRect);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(	viewportRect[0], viewportRect[2],
				viewportRect[1], viewportRect[3]);

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

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

	return Renderer::run(delta);
}
