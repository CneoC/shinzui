#pragma once

#ifndef __RESOURCES_SHADERRESOURCE_H__
#define __RESOURCES_SHADERRESOURCE_H__

#include "resources/Resource.h"

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class ShaderData
		: public ResourceData
	{
	public:
		static const char *getName()	{ return "Shader"; }

		//////////////////////////////////////////////////////////////////////////

		ShaderData(ResourceLoaderBase *pLoader)
			: ResourceData(pLoader)
		{
			getType() += getName();
		}

	protected:
	};

	//////////////////////////////////////////////////////////////////////////

	typedef ResourceRef<ShaderData>		ShaderResource;
}

#endif //__RESOURCES_SHADERRESOURCE_H__
