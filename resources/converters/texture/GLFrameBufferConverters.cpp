#include "GLFrameBufferConverters.h"

#include "resources/Resource.h"
#include "resources/texture/GLTextureResource.h"
#include "resources/texture/FITextureResource.h"
#include "resources/texture/GLFrameBufferResource.h"
#include "resources/texture/FrameBufferResource.h"

#include <FreeImage.h>

using namespace resources;
using namespace resources::converters;

//////////////////////////////////////////////////////////////////////////

Resource GLFrameBufferConverters::ConvertFromDef::convert(const Resource &res, const ResourceType &type)
{
	FrameBufferDef def(res, DONT_CONVERT);
	if (def && type & GLFrameBufferData::getName())
	{
		GLFrameBufferData *pData = new GLFrameBufferData(this);
		pData->setId(ResourceId(GLFrameBufferData::getName(), res->getId().getName()));
		pData->setSource(res);
		return GLFrameBufferResource(pData);
	}
	return Resource();
}

bool GLFrameBufferConverters::ConvertFromDef::load(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::load(res, flags))
		return true;

	if (!res->getSource()->isLoaded())
		return false;

	GLFrameBufferResource fb(res, DONT_CONVERT);
	FrameBufferDef def(fb->getSource(), DONT_CONVERT);

	GLTextureResource texture(fb);
	texture.load(flags | ResourceLoaderBase::FLAG_DONT_RECURSE);

	fb->setTexture(texture);

	GLuint depthFboId	= GL_NONE;
	GLuint fboId		= GL_NONE;

	if (GLEW_ARB_framebuffer_object)
	{
		// Create depth frame buffer
		if (def->isDepthAttached())
		{
			glGenRenderbuffers(1, &depthFboId);
			glBindRenderbuffer(GL_RENDERBUFFER, depthFboId);
			glRenderbufferStorage(
				GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
				def->getSize().x, def->getSize().y);
			// TODO: check FBO validity
		}

		// Create the frame buffer object
		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, texture->getTexture(), 0);

		// Attach depth buffer
		if (def->isDepthAttached())
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthFboId);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// TODO: check FBO validity
	}
	else if (GLEW_EXT_framebuffer_object)
	{
		// Create depth frame buffer
		if (def->isDepthAttached())
		{
			glGenRenderbuffersEXT(1, &depthFboId);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthFboId);
			glRenderbufferStorageEXT(
				GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24,
				def->getSize().x, def->getSize().y);
			// TODO: check FBO validity
		}

		// Create the frame buffer object
		glGenFramebuffersEXT(1, &fboId);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
		glFramebufferTexture2DEXT(
			GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
			GL_TEXTURE_2D, texture->getTexture(), 0);

		// Attach depth buffer
		if (def->isDepthAttached())
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthFboId);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		// TODO: check FBO validity
	}
	else
	{
		// TODO: Fall back to PBO?
		return false;
	}

	fb->setFBO(fboId);
	fb->setDepthFBO(depthFboId);

	fb->setLoaded(true);

	return true;
}

bool GLFrameBufferConverters::ConvertFromDef::unload(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::unload(res, flags))
		return true;

	GLFrameBufferResource fb(res, DONT_CONVERT);

	GLuint fboId = fb->getFBO();
	GLuint fboDepthId = fb->getDepthFBO();
	if (GLEW_ARB_framebuffer_object)
	{
		glDeleteFramebuffers(1, &fboId);
		glDeleteFramebuffers(1, &fboDepthId);
	}
	else if (GLEW_EXT_framebuffer_object)
	{
		glDeleteFramebuffersEXT(1, &fboId);
		glDeleteFramebuffersEXT(1, &fboDepthId);
	}

	fb->setLoaded(false);

	return true;
}