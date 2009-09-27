#pragma once

#ifndef __DEBUG_FONT_H__
#define __DEBUG_FONT_H__

#include "math/Math.h"
#include "math/Vector2.h"

#include <gl/glew.h>

#include <ft2build.h>
#include <freetype/freetype.h>

#pragma warning(disable: 4996)

class DebugFont
{
public:
	DebugFont();

	/**
	 * Load the debug font
	 */
	void load(const char *file, int height);
	/**
	 * Unload the debug font.
	 */
	void unload();

	void print(const Vector2f &p, size_t length, const char *text) const;

	/**
	 * Prints a single line of formatted text.
	 * @param p		translation to render the text at.
	 * @param fmt	format to print using variable argument list.
	 */
	void printf(const Vector2f &p, const char *fmt, ...) const;

protected:
	/**
	 * Creates a display list for a specific character.
	 */
	void createDisplayList(FT_Face face, unsigned char c);

protected:
	int		m_height;
	GLuint *m_pTextures;
	GLuint	m_displayLists;
};


#endif //__DEBUG_FONT_H__
