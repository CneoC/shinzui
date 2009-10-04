#include "Console.h"

#include <gl/glew.h>

using namespace console;

//////////////////////////////////////////////////////////////////////////

Console::Console(core::Core *pCore)
	: Renderer(pCore)
{
}

Console::~Console()
{
}

void Console::render(double delta)
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
}
