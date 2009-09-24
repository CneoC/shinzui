#include "Console.h"

#include <gl/glew.h>

Console::Console(Core *pCore)
	: Renderer(pCore)
{
	m_font.load("data/2d/fonts/debug.ttf", 14);
}

Console::~Console()
{
	m_font.unload();
}

Process *Console::run(double delta)
{
	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);

	GLint viewportRect[4];
	glGetIntegerv(GL_VIEWPORT, viewportRect);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(	viewportRect[0], viewportRect[2],
				viewportRect[1], viewportRect[3]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);
	m_font.printf(Vector2f(100, 100), "abcdefghijklmnopqrstuvwxyz");

	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(100.0f, 100.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f( 0.0f, 5.0f, 0.0f);
	glVertex3f(-5.0f,-5.0f, 0.0f);
	glVertex3f( 5.0f,-5.0f, 0.0f);
	glEnd();

	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();

	return Renderer::run(delta);
}
