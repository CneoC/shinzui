//////////////////////////////////////////////////////////////////////////
//
// This file is part of Shinzui.
//
// Shinzui is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shinzui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shinzui.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////
//
// Scene.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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
