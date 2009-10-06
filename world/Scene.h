#pragma once

#ifndef __WORLD_SCENE_H__
#define __WORLD_SCENE_H__

#include "render/Renderer.h"

#include "resources/texture/GLTextureResource.h"

namespace core
{
	class Core;
}

namespace world
{
	class Scene
		: public render::Renderer
	{
	public:
		Scene(core::Core *pCore);
		~Scene();

		virtual void render(double delta);

	protected:
		resources::GLTextureResource m_texture;
	};
}

#endif //__WORLD_SCENE_H__
