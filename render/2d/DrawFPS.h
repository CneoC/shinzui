#pragma once

#ifndef __DRAW_FPS_H__
#define __DRAW_FPS_H__

#include "render/Renderer.h"
#include "render/gl/2d/GLFont.h"

class DrawFPS
	: public Renderer
{
public:
	DrawFPS(Core *pCore);
	~DrawFPS();

	virtual Process *run(double delta);

	double getFPS() const	{ return m_fps; }

protected:
	GLFont		m_font;

	double		m_fps;
	u32			m_frameCount;
	double		m_frameTime;
};

#endif //__DRAW_FPS_H__
