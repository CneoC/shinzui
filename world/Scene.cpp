#include "Scene.h"

#include <gl/glew.h>

#include "resources/ResourceLoader.h"
#include "resources/texture/FITextureResource.h"

#include <math.h>

using namespace world;

//////////////////////////////////////////////////////////////////////////

Scene::Scene(core::Core *pCore)
	: render::Renderer(pCore)
{
	FITextureResource fiTexture(m_pCore->getLoader(), "File::2d/textures/test.tga", "FITexture::test");
	m_texture = GLTextureResource(m_pCore->getLoader(), "FITexture::test");
	m_texture.load();
}

Scene::~Scene()
{
}

void Scene::render(double delta)
{
	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	static float rotx = 0;
	static float roty = 0;
	rotx += 20 * (float)delta;
	roty += 10 * (float)delta;
	if (rotx > 360) rotx -= 360;
	if (roty > 360) roty -= 360;

	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(rotx, 1, 0, 0);
	glRotatef(roty, 0, 1, 0);

 	if (m_texture)
 	{
 		glEnable(GL_TEXTURE_2D);
 		m_texture->bind();
 	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glColor3f(1.0f, 1.0f, 1.0f);
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

	if (m_texture)
	{
		m_texture->unbind();
		glDisable(GL_TEXTURE_2D);
	}
}
