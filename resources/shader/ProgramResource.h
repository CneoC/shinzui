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
// ProgramResource.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCES_PROGRAMRESOURCE_H__
#define __RESOURCES_PROGRAMRESOURCE_H__

#include "resources/Resource.h"

#include <list>

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class ProgramData
		: public ResourceData
	{
	public:
		typedef std::list<std::string> ShaderList;

		static const char *getName()	{ return "Program"; }

		//////////////////////////////////////////////////////////////////////////

		ProgramData(ResourceLoaderBase *pLoader)
			: ResourceData(pLoader)
		{
			getType() += getName();
		}

		//////////////////////////////////////////////////////////////////////////

		virtual void bind() {}
		virtual void unbind() {}

		//////////////////////////////////////////////////////////////////////////

		void addShader(const std::string &shader)	{ m_shaders.push_back(shader); }
		void clearShaders()							{ m_shaders.clear(); }

		const ShaderList &getShaders() const		{ return m_shaders; }

	protected:
		ShaderList	m_shaders;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef ResourceRef<ProgramData>	ProgramResource;
}

#endif //__RESOURCES_PROGRAMRESOURCE_H__
