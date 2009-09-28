#pragma once

#ifndef __RENDER_GLFONT_H__
#define __RENDER_GLFONT_H__

#include "render/2d/Font.h"

#include "math/Math.h"
#include "math/Vector2.h"

#include "resources/font/GLFontResource.h"

#include <gl/glew.h>

#pragma warning(disable: 4996)

class GLFont
	: public Font
{
public:
	GLFont(GLFontResource font = Resource());

	/**
	 * Prints a single line of text.
	 * @param p			Translation to render the text at.
	 * @param length	Length of the text to render.
	 * @param text		Text to render.
	 */
	void print(const Vector2f &p, size_t length, const char *text) const;
};

#endif //__RENDER_GLFONT_H__
