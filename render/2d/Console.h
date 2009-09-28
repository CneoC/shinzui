#pragma once

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "render/Renderer.h"

class Console
	: public Renderer
{
public:
	Console(Core *pCore);
	~Console();

	virtual Process *run(double delta);

protected:
};

#endif //__CONSOLE_RENDER_PROC_H__