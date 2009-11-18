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
// GLProgramConverters.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "GLProgramConverters.h"

#include "resources/FileResource.h"
#include "resources/shader/GLShaderResource.h"
#include "resources/shader/GLProgramResource.h"

#include "util/logging/LogManager.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

using namespace resources;
using namespace resources::converters;

//////////////////////////////////////////////////////////////////////////

Resource GLProgramConverters::ConvertFromProgram::convert(const Resource &res, const ResourceType &type)
{
	ProgramResource def(res, DONT_CONVERT);
	if (def && type & GLProgramData::getName())
	{
		GLProgramData *pData = new GLProgramData(this);
		pData->setId(ResourceId(GLProgramData::getName(), res->getId().getName()));
		pData->setSource(res);
		return GLProgramResource(pData);
	}

	return Resource();
}

bool GLProgramConverters::ConvertFromProgram::load(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::load(res, flags))
		return true;

	logging::Log *log = LOG_GET("Resources.Converters.Program");

	GLProgramResource program(res);

	ProgramResource def = res->getSource();
	if (!def)
	{
		FileResource file = res->getSource();
		if (!file->isLoaded())
		{
			return false;
		}
	}
	else
	{
		program->setProgram(glCreateProgram());

		ResourceLoaderBase *pRootLoader = def->getLoader()->getRoot();

		ProgramData::ShaderList::const_iterator iter = def->getShaders().begin();
		while (iter != def->getShaders().end())
		{
			GLShaderResource shader = pRootLoader->get(*iter);
			if (shader.load())
				glAttachShader(program->getProgram(), shader->getShader());
			++iter;
		}
	}

	glLinkProgram(program->getProgram());

	GLint success;
	glGetProgramiv(program->getProgram(), GL_LINK_STATUS, &success);
	if (!success)
	{
		char buf[512];
		glGetProgramInfoLog(program, sizeof(buf), 0, buf);

		LOG_ERROR(log, "Unable to link program " << program->getId().toString() << ':' << std::endl << buf);

		return false;
	}

	LOG_TRACE(log, "Linked program: " << program->getId());
	program->setLoaded(true);

	return true;
}

bool GLProgramConverters::ConvertFromProgram::unload(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::unload(res, flags))
		return true;

	GLProgramResource program(res);

	glDeleteProgram(program->getProgram());
	program->setProgram(0);

	program->setLoaded(false);

	return true;
}
