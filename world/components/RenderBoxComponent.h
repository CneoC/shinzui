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
// RenderBoxComponent.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __WORLD_RENDERBOXCOMPONENT_H__
#define __WORLD_RENDERBOXCOMPONENT_H__

#include "render/Renderer.h"
#include "Component.h"

#include "TransformData.h"
#include "resources/texture/TextureResource.h"

#include "math/Vector3.h"

#include "os/current/gl/GLContext.h"

namespace world
{
	class RenderBoxComponent
		: public Component
		, public render::Renderer
	{
	public:
		RenderBoxComponent(core::Core *pCore, ComponentManager *pManager)
			: Component(pCore, pManager)
			, render::Renderer(pCore)
			, m_vertexVbo(0)
			, m_indexVbo(0)
		{
		}

		//////////////////////////////////////////////////////////////////////////

		virtual ComponentData *createData(const Entity &entity)
		{
			TransformData *pData = entity.getData()->create<TransformData>("Transform");
			m_transformHash = pData->_hash;
			return pData;
		}

		virtual void onStart() {}

		virtual void init();

		virtual void render(double delta);
		void renderBox(const math::Vector3f &position);

		const resources::TextureResource &getTexture() const		{ return m_texture; }
		void setTexture(const resources::TextureResource &texture)	{ m_texture = texture; }

	protected:
		resources::TextureResource m_texture;
		u32				m_transformHash;
		GLuint			m_vertexVbo;
		GLuint			m_indexVbo;
	};
}


#endif //__WORLD_RENDERBOXCOMPONENT_H__
