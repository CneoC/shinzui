#pragma once

#ifndef __RESOURCES_GLPROGRAMCONVERTERS_H__
#define __RESOURCES_GLPROGRAMCONVERTERS_H__

#include "resources/ResourceLoaderBase.h"

#include "resources/shader/GLShaderResource.h"
#include "resources/shader/GLProgramResource.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace fs = boost::filesystem;

namespace GLProgramConverters
{
	/**
	 * Loader class that can convert file resources to gl shader resources.
	 */
	class ConvertFromFile
		: public ResourceLoaderBase
	{
		virtual Resource convert(const Resource &res, const ResourceType &type)
		{
			FileResource file(res, DONT_CONVERT);
			GLProgramDefinition def(res, DONT_CONVERT);
			if ((file && (type & GLProgramData::getName() || file->getPath().extension() == ".glsl")) || def)
			{
				GLProgramData *pData = new GLProgramData(this);
				pData->setId(ResourceId(GLProgramData::getName(), res->getId().getName()));
				pData->setSource(res);
				return GLProgramResource(pData);
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

			GLProgramResource program(res);

			GLProgramDefinition def = res->getSource();
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

				GLProgramDataDef::ShaderList::const_iterator iter = def->getShaders().begin();
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

				logging::Log *log = LOG_GET("Resources.Converters.Program");
				LOG_ERROR(log, "Unable to link program: " << std::endl << buf);
				
				return false;
			}
			program->setLoaded(true);

			return true;
		}

		/**
		 * Unloads a resource (blocking).
		 */
		virtual bool unload(Resource &res, u32 flags)
		{
			if (ResourceLoaderBase::unload(res, flags))
				return true;

			GLProgramResource program(res);

			glDeleteProgram(program->getProgram());
			program->setProgram(0);

			program->setLoaded(false);

			return true;
		}
	};

} //namespace GLProgramConverters

#endif //__RESOURCES_GLPROGRAMCONVERTERS_H__
