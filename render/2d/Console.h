#pragma once

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "render/Renderer.h"
#include "render/2d/DebugFont.h"

class Console
	: public Renderer
{
public:
	Console(Core *pCore);
	~Console();

	virtual Process *run(double delta);

protected:
	DebugFont	m_font;
};

#endif //__CONSOLE_RENDER_PROC_H__