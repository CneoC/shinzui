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
// GLProgramResource.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCES_GLPROGRAMRESOURCE_H__
#define __RESOURCES_GLPROGRAMRESOURCE_H__

#include "ProgramResource.h"

#include <list>

#include <gl/glew.h>

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class GLProgramData
		: public ProgramData
	{
	public:
		static const char *getName()	{ return "GLProgram"; }

		//////////////////////////////////////////////////////////////////////////

		GLProgramData(ResourceLoaderBase *pLoader)
			: ProgramData(pLoader)
			, m_program(0)
		{
			getType() += getName();
			m_flags[FLAG_REQUIRE_CONTEXT] = true;
		}

		//////////////////////////////////////////////////////////////////////////

		virtual void bind()		{ glUseProgram(m_program); }
		virtual void unbind()	{ glUseProgram(0); }

		//////////////////////////////////////////////////////////////////////////

		void setProgram(GLuint program)		{ m_program = program; }
		GLuint getProgram() const			{ return m_program; }

	protected:
		GLuint		m_program;
	};

	typedef ResourceRef<GLProgramData>	GLProgramResource;
}

#endif //__RESOURCES_GLPROGRAMRESOURCE_H__
