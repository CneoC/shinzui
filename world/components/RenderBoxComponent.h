#pragma once

#ifndef __WORLD_RENDERBOXCOMPONENT_H__
#define __WORLD_RENDERBOXCOMPONENT_H__

#include "render/Renderer.h"
#include "Component.h"

#include "TransformData.h"
#include "resources/texture/TextureResource.h"

#include "math/Vector3.h"

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
		{
			m_displayList = 0;
		}

		//////////////////////////////////////////////////////////////////////////

		void renderBox(const math::Vector3f &position)
		{
			glPushMatrix();

			glTranslatef(position.x, position.y, position.z);

			glColor4f(
				min(-0.5 + fabs(position.x * 0.05f), 1), 
				min(-0.5 + fabs(position.y * 0.05f), 1),
				min(-0.5 + fabs(position.z * 0.05f), 1),
				1);
			glCallList(m_displayList);

			glPopMatrix();
		}

		virtual ComponentData *createData(const Entity &entity)
		{
			TransformData *pData = entity.getData()->create<TransformData>("Transform");
			m_transformHash = pData->_hash;
			return pData;
		}

		virtual void onStart() {}

		virtual void render(double delta)
		{
			if (!m_displayList)
			{
				m_displayList = glGenLists(1);
				glNewList(m_displayList, GL_COMPILE);

				glBegin(GL_QUADS);
				// front
				glTexCoord2f(0, 0); glVertex3f(-0.5f,-0.5f, 0.5f);
				glTexCoord2f(1, 0); glVertex3f( 0.5f,-0.5f, 0.5f);
				glTexCoord2f(1, 1); glVertex3f( 0.5f, 0.5f, 0.5f);
				glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f, 0.5f);

				// back
				glTexCoord2f(0, 0); glVertex3f(-0.5f,-0.5f,-0.5f);
				glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f,-0.5f);
				glTexCoord2f(1, 1); glVertex3f( 0.5f, 0.5f,-0.5f);
				glTexCoord2f(1, 0); glVertex3f( 0.5f,-0.5f,-0.5f);

				// top
				glTexCoord2f(0, 0); glVertex3f(-0.5f, 0.5f,-0.5f);
				glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
				glTexCoord2f(1, 1); glVertex3f( 0.5f, 0.5f, 0.5f);
				glTexCoord2f(1, 0); glVertex3f( 0.5f, 0.5f,-0.5f);

				// bottom
				glTexCoord2f(0, 0); glVertex3f(-0.5f,-0.5f,-0.5f);
				glTexCoord2f(1, 0); glVertex3f( 0.5f,-0.5f,-0.5f);
				glTexCoord2f(1, 1); glVertex3f( 0.5f,-0.5f, 0.5f);
				glTexCoord2f(0, 1); glVertex3f(-0.5f,-0.5f, 0.5f);

				// right
				glTexCoord2f(0, 0); glVertex3f( 0.5f,-0.5f,-0.5f);
				glTexCoord2f(1, 0); glVertex3f( 0.5f, 0.5f,-0.5f);
				glTexCoord2f(1, 1); glVertex3f( 0.5f, 0.5f, 0.5f);
				glTexCoord2f(0, 1); glVertex3f( 0.5f,-0.5f, 0.5f);

				// left
				glTexCoord2f(0, 0); glVertex3f(-0.5f,-0.5f,-0.5f);
				glTexCoord2f(0, 1); glVertex3f(-0.5f,-0.5f, 0.5f);
				glTexCoord2f(1, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
				glTexCoord2f(1, 0); glVertex3f(-0.5f, 0.5f,-0.5f);
				glEnd();

				glEndList();
			}

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
 
//			glEnable(GL_BLEND);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glTranslatef(0, 0, -100);

// 			static float rotx = 0;
 			static float roty = 0;
// 			rotx += 20 * (float)delta;
 			roty += 10 * (float)delta;
// 			if (rotx > 360) rotx -= 360;
 			if (roty > 360) roty -= 360;
 
// 			glRotatef(rotx, 1, 0, 0);
 			glRotatef(roty, 0, 1, 0);

			if (m_texture)
			{
				glEnable(GL_TEXTURE_2D);
				m_texture->bind();
			}

			EntityList::iterator entity;
			for (entity = m_entities.begin(); entity != m_entities.end(); ++entity)
			{
				TransformData *pData = static_cast<TransformData *>(entity->second.data);
				renderBox(pData->position);
			}

			if (m_texture)
			{
				m_texture->unbind();
				glDisable(GL_TEXTURE_2D);
			}
		}

		const TextureResource &getTexture() const		{ return m_texture; }
		void setTexture(const TextureResource &texture)	{ m_texture = texture; }

	protected:
		TextureResource m_texture;
		u32				m_transformHash;
		GLuint			m_displayList;
	};
}


#endif //__WORLD_RENDERBOXCOMPONENT_H__
