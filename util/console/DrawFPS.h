#pragma once

#ifndef __CONSOLE_DRAWFPS_H__
#define __CONSOLE_DRAWFPS_H__

#include "render/Renderer.h"
#include "render/2d/FontUtil.h"

#include "resources/font/FontResource.h"

namespace console
{
	class DrawFPS
		: public render::Renderer
	{
	public:
		DrawFPS(core::Core *pCore);
		~DrawFPS();

		virtual void render(double delta);

		double getFPS() const	{ return m_fps; }

	protected:
		render::FontUtil *m_pFontUtil;

		FontResource	m_font;

		double	m_fps;
		u32		m_frameCount;
		double	m_frameTime;
	};
}

#endif //__CONSOLE_DRAWFPS_H__
