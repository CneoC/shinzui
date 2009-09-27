#pragma once

#ifndef __GLFONT_H__
#define __GLFONT_H__

#include "math/Math.h"
#include "math/Vector2.h"

#include "resources/font/FTFontResource.h"
#include "resources/font/GLFontResource.h"

#include <gl/glew.h>

#pragma warning(disable: 4996)

class GLFont
{
public:
	GLFont(GLFontResource font = Resource());
	
	void setResource(GLFontResource &font) { m_font = font; }

	void print(const Vector2f &p, size_t length, const char *text) const;

	/**
	 * Prints a single line of formatted text.
	 * @param p		translation to render the text at.
	 * @param fmt	format to print using variable argument list.
	 */
	void printf(const Vector2f &p, const char *fmt, ...) const;

protected:
	GLFontResource m_font;
};


#endif //__GLFONT_H__
