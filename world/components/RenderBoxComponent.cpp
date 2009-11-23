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
// RenderBoxComponent.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "RenderBoxComponent.h"

using namespace world;

//////////////////////////////////////////////////////////////////////////

static GLfloat vertices[] = {
	-0.5f,-0.5f, 0.5f,
	0.5f,-0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f,
	0.5f, 0.5f,-0.5f,
	0.5f,-0.5f,-0.5f
};
static GLubyte texcoords[] = {
	0, 0,
	1, 0,
	1, 1,
	0, 1,
	0, 0,
	0, 1,
	1, 1,
	1, 0
};
static GLuint indices[] = {
	0, 1, 2, 3,
	4, 5, 6, 7,
	5, 3, 2, 6,
	4, 7, 1, 0,
	7, 6, 2, 1,
	4, 0, 3, 5
};

void RenderBoxComponent::init()
{
	if (GLEW_ARB_draw_buffers)
	{
		glGenBuffersARB(1, &m_vertexVbo);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertexVbo);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), vertices, GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		glGenBuffersARB(1, &m_indexVbo);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_indexVbo);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(indices), indices, GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}
}

void RenderBoxComponent::render(double delta)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

//			glEnable(GL_BLEND);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 10, -100);

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

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertexVbo);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_indexVbo);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	//glIndexPointer(GL_INT, 0, 0);

	EntityList::iterator entity;
	for (entity = m_entities.begin(); entity != m_entities.end(); ++entity)
	{
 		TransformData *pData = static_cast<TransformData *>(entity->second.data);
 		renderBox(pData->position);
		//glDrawElements(GL_QUADS, sizeof(indices), GL_UNSIGNED_BYTE, 0);
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

	if (m_texture)
	{
		m_texture->unbind();
		glDisable(GL_TEXTURE_2D);
	}
}


void RenderBoxComponent::renderBox(const math::Vector3f &position)
{
	glPushMatrix();

	float elapsed = (float)Renderer::m_pCore->getElapsedTime();

	glTranslatef(position.x, position.y, position.z);
	glScalef(
		2.0f + (cosf(position.x * 0.06f + elapsed * 0.6f) * 0.5f),
		2.0f + (cosf(position.y * 0.05f + elapsed * 0.7f) * 0.5f),
		2.0f + (cosf(position.z * 0.04f + elapsed * 0.8f) * 0.5f));

	glColor4f(
		(1.0f + cosf(position.x * 0.05f + elapsed * 1.0f)) * 0.5f,
		(1.0f + cosf(position.y * 0.06f + elapsed * 0.8f)) * 0.5f,
		(1.0f + cosf(position.z * 0.07f + elapsed * 0.5f)) * 0.5f,
		1.0f);
	//glCallList(m_displayList);
	glDrawElements(GL_QUADS, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_QUADS, 0, 4);

	glPopMatrix();
}