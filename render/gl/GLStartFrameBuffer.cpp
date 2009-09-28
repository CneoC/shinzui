#include "GLStartFrameBuffer.h"

#include <gl/glew.h>


class GLFrameBuffer
{
public:

};

GLStartFrameBuffer::GLStartFrameBuffer(Core *pCore)
	: Renderer(pCore)
{
	width = 800;
	height = 800;

	GLenum internalFormat = GL_RGBA;
	GLenum type = GL_UNSIGNED_BYTE;
	GLenum filter = GL_LINEAR;

	// create a color texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, type, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glBindTexture(GL_TEXTURE_2D, 0);
	//	phCheckError("Creation of the color texture for the FBO");

	// create depth renderbuffer
	if (false)//depth)
	{
		glGenRenderbuffersEXT(1, &depth);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height);
		//		phCheckError("Creation of the depth renderbuffer for the FBO");
	}
	else
	{
		depth = 0;
	}

	// create FBO itself
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);
	if (false)
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

GLStartFrameBuffer::~GLStartFrameBuffer()
{
}

Process *GLStartFrameBuffer::run(double delta)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 1);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	phCheckFBO();
//	phCheckError("Creation of the FBO itself");

	return Renderer::run(delta);
}
