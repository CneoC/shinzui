#pragma once

#ifndef __SCENE_SCENE_H__
#define __SCENE_SCENE_H__

#include "render/Renderer.h"

#include "resources/texture/GLTextureResource.h"

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
		GLTextureResource m_texture;
	};
}

#endif //__SCENE_SCENE_H__
