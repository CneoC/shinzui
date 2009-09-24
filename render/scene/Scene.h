#pragma once

#ifndef __SCENE_H__
#define __SCENE_H__

#include "render/Renderer.h"

class Scene
	: public Renderer
{
public:
	Scene(Core *pCore);
	~Scene();

	virtual Process *run(double delta);

protected:
};

#endif //__SCENE_H__
