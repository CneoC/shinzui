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
// GLShaderConverters.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "GLShaderConverters.h"

#include "resources/FileResource.h"
#include "resources/shader/ShaderResource.h"
#include "resources/shader/GLShaderResource.h"

#include "util/logging/LogManager.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

using namespace resources;
using namespace resources::converters;

//////////////////////////////////////////////////////////////////////////

Resource GLShaderConverters::ConvertFromFile::convert(const Resource &res, const ResourceType &type)
{
	FileResource file(res, DONT_CONVERT);
	if (file)
	{
		GLShaderData::ShaderType shaderType = GLShaderData::TYPE_NONE;
		if (type & GLShaderData::getVertName() || file->getPath().extension() == ".vert")
			shaderType = GLShaderData::TYPE_VERTEX;
		else if (type & GLShaderData::getFragName() || file->getPath().extension() == ".frag")
			shaderType = GLShaderData::TYPE_FRAGMENT;

		if (shaderType != GLShaderData::TYPE_NONE)
		{
			GLShaderData *pData = new GLShaderData(this);
			pData->setId(ResourceId(GLShaderData::getName(), res->getId().getName()));
			pData->setSource(res);
			pData->setShaderType(shaderType);
			return GLShaderResource(pData);
		}
	}
	return Resource();
}

bool GLShaderConverters::ConvertFromFile::load(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::load(res, flags))
		return true;

	FileResource file = res->getSource();
	if (!file->isLoaded())
		return false;

	GLShaderResource shader(res);

	u32 type = 0;
	switch (shader->getShaderType())
	{
	case GLShaderData::TYPE_VERTEX:		type = GL_VERTEX_SHADER; break;
	case GLShaderData::TYPE_FRAGMENT:	type = GL_FRAGMENT_SHADER; break;
	}

	// Open the file
	std::ifstream f;
	f.open(file->getPath().native_file_string().c_str(), std::ios::in);

	// Calculate file length
	f.seekg(0, std::ios::end);
	u32 length = f.tellg();
	f.seekg(std::ios::beg);

	// Read the file
	GLchar *program = new char[length];
	f.read(program, length);
	length = f.gcount();

	// Compile the shader
	shader->setShader(glCreateShader(type));
	glShaderSource(shader->getShader(), 1, (const GLchar**)&program, (const GLint *)&length);
	glCompileShader(shader->getShader());

	// Check for errors
	GLint success;
	glGetShaderiv(shader->getShader(), GL_COMPILE_STATUS, &success);
	delete [] program;
	if (!success)
	{
		char buf[512];
		glGetShaderInfoLog(shader->getShader(), sizeof(buf), 0, buf);

		logging::Log *log = LOG_GET("Resources.Converters.Shader");
		LOG_ERROR(log, "Unable to compile shader '" << file->getPath() << "': " << std::endl << buf);

		return false;
	}

	shader->setLoaded(true);

	return true;
}

bool GLShaderConverters::ConvertFromFile::unload(Resource &res, u32 flags)
{
	if (ResourceLoaderBase::unload(res, flags))
		return true;

	GLShaderResource shader(res);

	glDeleteShader(shader->getShader());
	shader->setShader(0);

	shader->setLoaded(false);

	return true;
}