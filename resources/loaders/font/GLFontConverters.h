#pragma once

#ifndef __RESOURCES_GLFONTCONVERTERS_H__
#define __RESOURCES_GLFONTCONVERTERS_H__

#include "resources/ResourceLoaderBase.h"

#include "resources/font/FTFontResource.h"
#include "resources/font/GLFontResource.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/fttrigon.h>
#include <freetype/ftoutln.h>
#include <freetype/ftbitmap.h>
#include <freetype/fttypes.h>

namespace fs = boost::filesystem;

namespace GLFontConverters
{
	/**
	 * Loader class that can convert freetype2 font resources to OpenGL font resources.
	 */
	class ConvertFromFT
		: public ResourceLoaderBase
	{
		virtual Resource convert(const Resource &res, ResourceType type)
		{
			FTFontResource font(res, false);
			if (font && type == RESOURCE_GL_FONT)
			{
				GLFontData *pData = new GLFontData(this);
				pData->setId(res->getId());
				pData->setSource(res);
				return GLFontResource(pData);
			}
			return Resource();
		}

		/**
		 * Loads the OpenGL font resource (blocking).
		 * @return if resource was properly loaded.
		 */
		virtual bool load(Resource &res, u32 flags)
		{
			if (ResourceLoaderBase::load(res, flags))
				return true;

			if (!res->getSource()->isLoaded())
				return false;

			GLFontResource font(res);

			font->setLoaded(true);

			// Allocate GL resources
			font->setTextures(new GLuint[128]);
			glGenTextures(128, font->getTextures());

			font->setDisplayLists(glGenLists(128));

			// Generate font display lists
			for (unsigned char c = 0; c < 128; c++)
			{
				createDisplayList(font, c);
			}

			return true;
		}

		/**
		 * Unloads a resource (blocking).
		 */
		virtual bool unload(Resource &res, u32 flags)
		{
			if (ResourceLoaderBase::unload(res, flags))
				return true;

			GLFontResource font(res);

			font->setLoaded(false);

			return true;
		}

		void createDisplayList(GLFontResource &font, unsigned char c)
		{
			FTFontResource ftFont(font->getSource());

			if (!ftFont)
				throw std::runtime_error("Unable to get FTFontResource.");

			FT_Face face = ftFont->getFace();

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
			GLubyte *pTextureData = new GLubyte[(width * height) << 1];
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
			glBindTexture(GL_TEXTURE_2D, font->getTexture(c));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// Use copied texture data to generate the actual OpenGL texture.
			glTexImage2D(GL_TEXTURE_2D, 0,
				GL_RGBA, width, height, 0,
				GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,
				pTextureData);

			delete [] pTextureData;

			// Create the character display list
			glNewList(font->getDisplayLists() + c, GL_COMPILE);
			glBindTexture(GL_TEXTURE_2D, font->getTexture(c));
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

	};
} //namespace GLFontConverters

#endif //__RESOURCES_GLFONTCONVERTERS_H__
