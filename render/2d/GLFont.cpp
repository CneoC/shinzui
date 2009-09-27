#include "GLFont.h"

#include <freetype/ftglyph.h>
#include <freetype/fttrigon.h>
#include <freetype/ftoutln.h>
#include <freetype/ftbitmap.h>
#include <freetype/fttypes.h>

#include <assert.h>
#include <stdexcept>
#include <string>

GLFont::GLFont(GLFontResource font)
	: m_font(font)
{
}

void GLFont::print(const Vector2f &p, size_t length, const char *text) const
{
	// Set proper render settings for font rendering.
	glPushAttrib(GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_ENABLE_BIT | GL_LIST_BIT); 
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(p.x, p.y, 0);

	// Call the display lists for each character in the formatted text
	glListBase(m_font->getDisplayLists());
	glCallLists(length, GL_UNSIGNED_BYTE, text);

	glPopMatrix();
	glPopAttrib();
}

void GLFont::printf(const Vector2f &p, const char *fmt, ...) const
{
	assert(fmt);

	// Parse and print variable argument list.
	char	text[512];
	va_list	ap;
	va_start(ap, fmt);
	size_t length = vsprintf(text, fmt, ap);
	va_end(ap);

	print(p, length, text);
}
