#pragma once

#ifndef __GL_WINDOW_H__
#define __GL_WINDOW_H__

#include "Win32Window.h"

class GLWindow :
	public Win32Window
{
public:
	GLWindow(Core *pCore) : Win32Window(pCore) {}
	~GLWindow() {}

	virtual Process *run(double delta);

protected:
	bool init();

	virtual bool createContext();
	virtual bool destroyContext();
	virtual bool resizeContext();
};

#endif //__GL_WINDOW_H__
