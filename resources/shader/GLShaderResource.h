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
// GLShaderResource.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCES_GLSHADERRESOURCE_H__
#define __RESOURCES_GLSHADERRESOURCE_H__

#include "ShaderResource.h"

#include "os/current/gl/GLContext.h"

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class GLShaderData
		: public ShaderData
	{
	public:
		enum ShaderType
		{
			TYPE_NONE,
			TYPE_VERTEX,
			TYPE_FRAGMENT
		};

		static const char *getName()		{ return "GLShader"; }
		static const char *getVertName()	{ return "GLVertShader"; }
		static const char *getFragName()	{ return "GLFragShader"; }

		//////////////////////////////////////////////////////////////////////////

		GLShaderData(ResourceLoaderBase *pLoader)
			: ShaderData(pLoader)
			, m_shaderType(TYPE_NONE)
			, m_shader(0)
		{
			getType() += getName();
			m_flags[FLAG_REQUIRE_CONTEXT] = true;
		}

		//////////////////////////////////////////////////////////////////////////

		void setShaderType(ShaderType type)	{ m_shaderType = type; }
		ShaderType getShaderType() const	{ return m_shaderType; }

		void setShader(GLuint shader)		{ m_shader = shader; }
		GLuint getShader() const			{ return m_shader; }

	protected:
		ShaderType	m_shaderType;
		GLuint		m_shader;
	};

	typedef ResourceRef<GLShaderData>	GLShaderResource;
}

#endif //__RESOURCES_GLSHADERRESOURCE_H__
