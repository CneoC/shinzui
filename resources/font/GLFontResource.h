#pragma once

#ifndef __RESOURCES_GLFONTRESOURCE_H__
#define __RESOURCES_GLFONTRESOURCE_H__

#include "FontResource.h"

#include <gl/glew.h>

class GLFontData
	: public FontData
{
public:
	static const char *getName()	{ return "GLFont"; }

	GLFontData(ResourceLoaderBase *pData)
		: FontData(pData)
	{
		getType() += getName();
		m_flags[FLAG_REQUIRE_CONTEXT] = true;
	}

	void setTextures(GLuint *pTextures)	{ m_pTextures = pTextures; }
	GLuint *getTextures()				{ return m_pTextures; }
	GLuint getTexture(char c)			{ return m_pTextures[c]; }

	void setDisplayLists(GLuint lists)	{  m_displayLists = lists; }
	GLuint getDisplayLists() const		{ return m_displayLists; }

protected:
	GLuint *m_pTextures;
	GLuint	m_displayLists;

};

typedef ResourceRef<GLFontData>		GLFontResource;

#endif //__RESOURCES_GLFONTRESOURCE_H__
