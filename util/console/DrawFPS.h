#pragma once

#ifndef __DRAW_FPS_H__
#define __DRAW_FPS_H__

#include "render/Renderer.h"
#include "render/2d/FontUtil.h"

class DrawFPS
	: public render::Renderer
{
public:
	DrawFPS(Core *pCore);
	~DrawFPS();

	virtual void render(double delta);

	double getFPS() const	{ return m_fps; }

protected:
	render::FontUtil *m_pFontUtil;

	double	m_fps;
	u32		m_frameCount;
	double	m_frameTime;
};

#endif //__DRAW_FPS_H__
