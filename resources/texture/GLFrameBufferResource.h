#pragma once

#ifndef __RESOURCES_GLFRAMEBUFFERRESOURCE_H__
#define __RESOURCES_GLFRAMEBUFFERRESOURCE_H__

#include "FrameBufferResource.h"

#include <gl/glew.h>

class GLFrameBufferData
	: public FrameBufferData
{
public:
	static const char *getName()	{ return "GLFrameBuffer"; }

	GLFrameBufferData(ResourceLoaderBase *pData)
		: FrameBufferData(pData)
		, m_fbo(0)
		, m_depthFbo(0)
	{
		getType() += getName();
	}

	void setFBO(GLuint fbo)			{ m_fbo = fbo; }
	GLuint getFBO()					{ return m_fbo; }

	void setDepthFBO(GLuint fbo)	{ m_depthFbo = fbo; }
	GLuint getDepthFBO()			{ return m_depthFbo; }

	virtual void bind()
	{
		if (GLEW_ARB_framebuffer_object)
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		else if (GLEW_EXT_framebuffer_object)
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

		GLuint clearFlags = GL_NONE;
		if (getDef()->isCleared())
			clearFlags |= GL_COLOR_BUFFER_BIT;
		if (getDef()->isDepthCleared())
			clearFlags |= GL_DEPTH_BUFFER_BIT;
		if (clearFlags)
		{
			math::Color4f c = getDef()->getClearColor();
			glClearColor(c.r, c.g, c.b, c.a);
			glClear(clearFlags);
		}
	}

	virtual void unbind()
	{
		if (GLEW_ARB_framebuffer_object)
			glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
		else if (GLEW_EXT_framebuffer_object)
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, GL_NONE);
	}

protected:
	GLuint			m_fbo;
	GLuint			m_depthFbo;
};

typedef ResourceRef<GLFrameBufferData>		GLFrameBufferResource;

#endif //__RESOURCES_GLFRAMEBUFFERRESOURCE_H__
