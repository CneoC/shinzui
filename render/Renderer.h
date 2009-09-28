#pragma once

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "core/ProcessChain.h"

#include "RenderFactory.h"

class Renderer :
	public ProcessChain
{
protected:
	Renderer(Core *pCore, int id = 0, int targetThreadId = Core::THREAD_ID_MAIN_BIT);
	~Renderer();

public:

protected:
};

#endif //__RENDERER_H__
