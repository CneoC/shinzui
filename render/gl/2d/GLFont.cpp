#include "GLFont.h"

#include "resources/ResourceLoaderBase.h"

#include <stdexcept>

GLFont::GLFont(GLFontResource font)
	: Font(font)
{
}

void GLFont::print(const Vector2f &p, size_t length, const char *text) const
{
	GLFontResource font(m_font);
	if (!font) throw std::runtime_error("Invalid font resource, expected GLFontResource.");

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
	glListBase(font->getDisplayLists());
	glCallLists(length, GL_UNSIGNED_BYTE, text);

	glPopMatrix();
	glPopAttrib();
}
