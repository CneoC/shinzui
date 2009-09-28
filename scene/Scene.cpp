#include "Scene.h"

#include <gl/glew.h>

#define _USE_MATH_DEFINES
#include <math.h>

Scene::Scene(Core *pCore)
	: Renderer(pCore)
{
}

Scene::~Scene()
{
}

Process *Scene::run(double delta)
{
	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	static double rotx = 0;
	static double roty = 0;
	rotx += 20 * delta;
	roty += 10 * delta;
	if (rotx > 360) rotx -= 360;
	if (roty > 360) roty -= 360;

	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(rotx, 1, 0, 0);
	glRotatef(roty, 0, 1, 0);

	glDisable(GL_TEXTURE_2D);


	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
		// front
		glVertex3f(-0.5f,-0.5f, 0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f( 0.5f, 0.5f, 0.5f);
		glVertex3f( 0.5f,-0.5f, 0.5f);

		// back
		glVertex3f(-0.5f,-0.5f,-0.5f);
		glVertex3f( 0.5f,-0.5f,-0.5f);
		glVertex3f( 0.5f, 0.5f,-0.5f);
		glVertex3f(-0.5f, 0.5f,-0.5f);

		// top
		glVertex3f(-0.5f, 0.5f,-0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f( 0.5f, 0.5f, 0.5f);
		glVertex3f( 0.5f, 0.5f,-0.5f);

		// bottom
		glVertex3f(-0.5f,-0.5f,-0.5f);
		glVertex3f( 0.5f,-0.5f,-0.5f);
		glVertex3f( 0.5f,-0.5f, 0.5f);
		glVertex3f(-0.5f,-0.5f, 0.5f);

		// right
		glVertex3f( 0.5f,-0.5f,-0.5f);
		glVertex3f( 0.5f,-0.5f, 0.5f);
		glVertex3f( 0.5f, 0.5f, 0.5f);
		glVertex3f( 0.5f, 0.5f,-0.5f);

		// left
		glVertex3f(-0.5f,-0.5f,-0.5f);
		glVertex3f(-0.5f, 0.5f,-0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f,-0.5f, 0.5f);
	glEnd();

	return Renderer::run(delta);
}
