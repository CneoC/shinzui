#pragma once

#ifndef __RESOURCES_FONT_GLFONTRESOURCE_H__
#define __RESOURCES_FONT_GLFONTRESOURCE_H__

#include "FontResource.h"

class GLFontData
	: public FontData
{
public:
	enum { TYPE = RESOURCE_GL_FONT };

	GLFontData(ResourceLoaderBase *pData)
		: FontData(pData)
	{
		setType(TYPE);
	}

protected:
};

typedef ResourceRef<GLFontData>		GLFontResource;

#endif //__RESOURCES_FONT_GLFONTRESOURCE_H__
