//////////////////////////////////////////////////////////////////////////
//
// This file is part of Shinzui.
//
// Shinzui is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shinzui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shinzui.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////
//
// GLFrameBufferConverters.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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

	GLuint depthRboId	= GL_NONE;
	GLuint fboId		= GL_NONE;

	if (GLEW_ARB_framebuffer_object)
	{
		// Create depth frame buffer
		if (def->isDepthAttached())
		{
			glGenRenderbuffers(1, &depthRboId);
			glBindRenderbuffer(GL_RENDERBUFFER, depthRboId);
			glRenderbufferStorage(
				GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
				def->getSize().x, def->getSize().y);
		}

		// Create the frame buffer object
		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, texture->getTexture(), 0);

		// Attach depth buffer
		if (def->isDepthAttached())
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRboId);

		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			throw std::runtime_error("Unable to properly create FBO");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else if (GLEW_EXT_framebuffer_object)
	{
		// Create depth frame buffer
		if (def->isDepthAttached())
		{
			glGenRenderbuffersEXT(1, &depthRboId);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRboId);
			glRenderbufferStorageEXT(
				GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
				def->getSize().x, def->getSize().y);
		}

		// Create the frame buffer object
		glGenFramebuffersEXT(1, &fboId);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
		glFramebufferTexture2DEXT(
			GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
			GL_TEXTURE_2D, texture->getTexture(), 0);

		// Attach depth buffer
		if (def->isDepthAttached())
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthRboId);

		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			throw std::runtime_error("Unable to properly create FBO");

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
	else
	{
		// TODO: Fall back to PBO?
		return false;
	}

	fb->setFBO(fboId);
	fb->setDepthRBO(depthRboId);

	fb->setLoaded(true);

	return true;
}

bool GLFrameBufferConverters::ConvertFromDef::unload(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::unload(res, flags))
		return true;

	GLFrameBufferResource fb(res, DONT_CONVERT);

	GLuint fboId = fb->getFBO();
	GLuint rboDepthId = fb->getDepthRBO();
	if (GLEW_ARB_framebuffer_object)
	{
		glDeleteFramebuffers(1, &fboId);
		glDeleteRenderbuffers(1, &rboDepthId);
	}
	else if (GLEW_EXT_framebuffer_object)
	{
		glDeleteFramebuffersEXT(1, &fboId);
		glDeleteRenderbuffersEXT(1, &rboDepthId);
	}

	fb->setLoaded(false);

	return true;
}