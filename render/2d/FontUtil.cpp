#include "FontUtil.h"

#include <assert.h>
#include <stdarg.h>

using namespace render;

#pragma warning(disable: 4996)

FontUtil::FontUtil(FontResource font)
	: m_font(font)
{
}

void FontUtil::printf(const Vector2f &p, const char *fmt, ...) const
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
