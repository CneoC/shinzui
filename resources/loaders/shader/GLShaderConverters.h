#pragma once

#ifndef __RESOURCES_GLSHADERCONVERTERS_H__
#define __RESOURCES_GLSHADERCONVERTERS_H__

#include "resources/ResourceLoaderBase.h"

#include "resources/shader/ShaderResource.h"
#include "resources/shader/GLShaderResource.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

namespace GLShaderConverters
{
	/**
	 * Loader class that can convert file resources to gl shader resources.
	 */
	class ConvertFromFile
		: public ResourceLoaderBase
	{
		virtual Resource convert(const Resource &res, ResourceType type)
		{
			FileResource file(res, false);
			if (file)
			{
				GLShaderData::ShaderType type = GLShaderData::TYPE_NONE;
				if (type == RESOURCE_GL_VERT_SHADER || file->getPath().extension() == ".vert")
					type = GLShaderData::TYPE_VERTEX;
				else if (type == RESOURCE_GL_FRAG_SHADER || file->getPath().extension() == ".frag")
					type = GLShaderData::TYPE_FRAGMENT;

				if (type != GLShaderData::TYPE_NONE)
				{
					GLShaderData *pData = new GLShaderData(this);
					pData->setId(res->getId());
					pData->setSource(res);
					pData->setShaderType(type);
					return GLShaderResource(pData);
				}
			}
			return Resource();
		}

		/**
		 * Loads the OpenGL shader resource (blocking).
		 * @return if resource was properly loaded.
		 */
		virtual bool load(Resource &res, u32 flags)
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

		/**
		 * Unloads a resource (blocking).
		 */
		virtual bool unload(Resource &res, u32 flags)
		{
			if (ResourceLoaderBase::unload(res, flags))
				return true;

			GLShaderResource shader(res);

			glDeleteShader(shader->getShader());
			shader->setShader(0);

			shader->setLoaded(false);

			return true;
		}
	};

} //namespace GLShaderConverters

#endif //__RESOURCES_GLSHADERCONVERTERS_H__
