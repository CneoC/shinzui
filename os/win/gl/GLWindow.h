#pragma once

#ifndef __GL_WINDOW_H__
#define __GL_WINDOW_H__

#include "os/win/Window.h"

class GLWindow :
	public Window
{
public:
	GLWindow(Core *pCore) : Window(pCore) {}
	~GLWindow() {}

	virtual Process *run(double delta);

protected:
	bool init();

	virtual bool createContext();
	virtual bool destroyContext();
	virtual bool resizeContext();
};

#endif //__GL_WINDOW_H__
