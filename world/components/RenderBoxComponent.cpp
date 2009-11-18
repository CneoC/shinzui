#include "RenderBoxComponent.h"

#include <gl/glew.h>

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

	glTranslatef(position.x, position.y, position.z);

	double elapsed = Renderer::m_pCore->getElapsedTime();
	glColor4f(
		(1 + cos(position.x * 0.05f + elapsed * 1.0f)) * 0.5,
		(1 + cos(position.y * 0.06f + elapsed * 0.8f)) * 0.5,
		(1 + cos(position.z * 0.07f + elapsed * 0.5f)) * 0.5,
		1);
	//glCallList(m_displayList);
	glDrawElements(GL_QUADS, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_QUADS, 0, 4);

	glPopMatrix();
}
