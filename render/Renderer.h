#pragma once

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "core/ChainProcess.h"

class Renderer :
	public ChainProcess
{
protected:
	Renderer(Core *pCore, int id = 0, int targetThreadId = Core::THREAD_ID_MAIN);
	~Renderer();

public:

protected:
};

#endif //__RENDERER_H__
