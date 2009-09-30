#pragma once

#ifndef __SCENE_SCENE_H__
#define __SCENE_SCENE_H__

#include "render/Renderer.h"

class Core;

namespace scene
{
	class Scene
		: public render::Renderer
	{
	public:
		Scene(Core *pCore);
		~Scene();

		virtual void render(double delta);

	protected:
	};
}

#endif //__SCENE_SCENE_H__
