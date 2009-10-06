#pragma once

#ifndef __WORLD_RENDERBOXCOMPONENT_H__
#define __WORLD_RENDERBOXCOMPONENT_H__

#include "render/Renderer.h"
#include "Component.h"

#include "TransformData.h"
#include "resources/texture/TextureResource.h"

#include "math/Vector3.h"

#include <gl/glew.h>

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
