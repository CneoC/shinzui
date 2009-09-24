#pragma once

#ifndef __DEBUG_FONT_H__
#define __DEBUG_FONT_H__

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/fttrigon.h>
#include <freetype/ftoutln.h>
#include <freetype/ftbitmap.h>
#include <freetype/fttypes.h>

#include <gl/glew.h>

#include "math/Math.h"
#include "math/Vector2.h"

#pragma warning(disable: 4996)

class DebugFont
{
public:
	DebugFont()
		: m_pTextures(NULL)
	{
	}

	/**
	 * Load the debug font
	 */
	void load(const char *file, int height)
	{
		assert(!m_pTextures);

		m_height = height;

		FT_Library library;
		if (FT_Init_FreeType(&library))
		{
			throw std::runtime_error("FT_Init_FreeType failed.");
		}

		FT_Face face;
		if (FT_New_Face(library, file, 0, &face))
		{
			throw std::runtime_error(std::string("FT_New_Face failed. file '") + file + "' probably doesn't exist.");
		}

		FT_Set_Char_Size(face, m_height << 6, m_height << 6, 96, 96);

		// Allocate GL resources
		m_pTextures = new GLuint[128];
		glGenTextures(128, m_pTextures);
		m_displayLists = glGenLists(128);

		// Generate font display lists
		for (unsigned char c = 0; c < 128; c++)
		{
			createDisplayList(face, c);
		}

		// Done with freetype
		FT_Done_Face(face);
		FT_Done_FreeType(library);
	}

	/**
	 * Unload the debug font.
	 */
	void unload()
	{
		glDeleteLists(m_displayLists, 128);
		glDeleteTextures(128, m_pTextures);

		delete [] m_pTextures;
		m_pTextures = NULL;
	}

	void print(const Vector2f &p, size_t length, const char *text) const
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
		glListBase(m_displayLists);
		glCallLists(length, GL_UNSIGNED_BYTE, text);

		glPopMatrix();
		glPopAttrib();
	}

	/**
	 * Prints a single line of formatted text.
	 * @param p		translation to render the text at.
	 * @param fmt	format to print using variable argument list.
	 */
	void printf(const Vector2f &p, const char *fmt, ...) const
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

protected:
	/**
	 * Creates a display list for a specific character.
	 */
	void createDisplayList(FT_Face face, unsigned char c)
	{
		// Load the character glyph.
		if (FT_Load_Glyph(face, FT_Get_Char_Index(face, c), FT_LOAD_DEFAULT))
		{
			throw std::runtime_error("FT_Load_Glyph failed.");
		}

		// Get the glyph data.
		FT_Glyph glyph;
		if (FT_Get_Glyph(face->glyph, &glyph))
		{
			throw std::runtime_error("FT_Get_Glyph failed.");
		}

		// Convert to bitmap.
		if (glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			if (FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1))
			{
				throw std::runtime_error("FT_Glyph_To_Bitmap failed.");
			}
		}
		FT_BitmapGlyph pBitmapGlyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap &bitmap = pBitmapGlyph->bitmap;

		int width = Math::nextPower2(bitmap.width);
		int height = Math::nextPower2(bitmap.rows);

		// Copy glyph texture data to a OpenGL texture format with both alpha and luminosity channels.
		//::printf("Char: %c\n", c);
		GLubyte* pTextureData = new GLubyte[(width * height) << 1];
		for (int y = 0; y < height; y++) 
		{
			for (int x = 0; x < width; x++)
			{
				const int src = x + y * bitmap.width;
				const int dst = (x + y * width) << 1;
				const bool padding = x >= bitmap.width || y >= bitmap.rows;
				pTextureData[dst] = pTextureData[dst + 1] = padding ? 0 : bitmap.buffer[src];

				//if (!padding) ::printf("%1i", bitmap.buffer[src] / 32);
			}
			//::printf("\n");
		}
		//::printf("\n\n");

		// Prepare OpenGL texture.
		glBindTexture(GL_TEXTURE_2D, m_pTextures[c]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Use copied texture data to generate the actual OpenGL texture.
		glTexImage2D(GL_TEXTURE_2D, 0,
					 GL_RGBA, width, height, 0,
					 GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,
					 pTextureData);

		delete [] pTextureData;


		// Create the character display list
		glNewList(m_displayLists + c, GL_COMPILE);
			glBindTexture(GL_TEXTURE_2D, m_pTextures[c]);
			glPushMatrix();
				// Offset character properly.
				glTranslatef((float)pBitmapGlyph->left,
							 (float)(pBitmapGlyph->top - bitmap.rows),
							 0);

				// Calculate texture coordinates.
				float x = (float)bitmap.width / (float)width;
				float y = (float)bitmap.rows / (float)height;

				// Generate quad to render character.
				glBegin(GL_QUADS);
					glTexCoord2d(0, 0); glVertex2i(0, bitmap.rows);
					glTexCoord2d(0, y); glVertex2i(0, 0);
					glTexCoord2d(x, y); glVertex2i(bitmap.width, 0);
					glTexCoord2d(x, 0); glVertex2i(bitmap.width, bitmap.rows);
				glEnd();
			glPopMatrix();

			// Position for next character.
			glTranslatef((float)(face->glyph->advance.x >> 6), 0, 0);
		
		glEndList(); // m_displayLists + c

		FT_Done_Glyph(glyph);
	}

protected:
	int		m_height;
	GLuint	*m_pTextures;
	GLuint	m_displayLists;
};


#endif //__DEBUG_FONT_H__