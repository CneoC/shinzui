#pragma once

#ifndef __RENDER_FONTUTIL_H__
#define __RENDER_FONTUTIL_H__

#include "render/RenderUtil.h"

#include "math/Math.h"
#include "math/Vector2.h"

#include "resources/font/FontResource.h"

#include <gl/glew.h>

namespace render
{
	class FontUtil
		: public RenderUtil
	{
	public:
		FontUtil(FontResource font = Resource());

		void setResource(FontResource font) { m_font = font; }

		/**
		 * Prints a single line of text.
		 * @param p			Translation to render the text at.
		 * @param length	Length of the text to render.
		 * @param text		Text to render.
		 */
		virtual void print(const math::Vector2f &p, size_t length, const char *text) const = 0;

		/**
		 * Prints a single line of formatted text.
		 * @param p		translation to render the text at.
		 * @param fmt	format to print using variable argument list.
		 */
		void printf(const math::Vector2f &p, const char *fmt, ...) const;

	protected:
		FontResource m_font;
	};
}


#endif //__RENDER_FONTUTIL_H__
