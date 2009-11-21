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
// GLFrameBufferResource.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCES_GLFRAMEBUFFERRESOURCE_H__
#define __RESOURCES_GLFRAMEBUFFERRESOURCE_H__

#include "FrameBufferResource.h"

#include "os/current/gl/GLContext.h"

namespace resources
{
	class GLFrameBufferData
		: public FrameBufferData
	{
	public:
		static const char *getName()	{ return "GLFrameBuffer"; }

		GLFrameBufferData(ResourceLoaderBase *pData)
			: FrameBufferData(pData)
			, m_fbo(0)
			, m_depthRbo(0)
		{
			getType() += getName();
			m_flags[FLAG_REQUIRE_CONTEXT] = true;
		}

		void setFBO(GLuint fbo)			{ m_fbo = fbo; }
		GLuint getFBO()					{ return m_fbo; }

		void setDepthRBO(GLuint rbo)	{ m_depthRbo = rbo; }
		GLuint getDepthRBO()			{ return m_depthRbo; }

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
		GLuint			m_depthRbo;
	};

	typedef ResourceRef<GLFrameBufferData>		GLFrameBufferResource;
}

#endif //__RESOURCES_GLFRAMEBUFFERRESOURCE_H__
