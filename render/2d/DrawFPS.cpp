#include "DrawFPS.h"

#include "resources/ResourceLoader.h"
#include "resources/font/GLFontResource.h"
#include "resources/font/FTFontResource.h"

#include <gl/glew.h>

DrawFPS::DrawFPS(Core *pCore)
	: Renderer(pCore)
	, m_frameCount(0)
	, m_frameTime(0)
	, m_fps(0)
{
	GLFontResource font = pCore->getLoader()->get("2d/fonts/debug.ttf");
	FTFontResource ftFont = font->getSource();
	ftFont->setSize(16);
	font.load();
	m_font.setResource(font);
}

DrawFPS::~DrawFPS()
{
}

Process *DrawFPS::run(double delta)
{
// 	FontResource debugFont = m_pCore->getResourceLoader()->getFont("2d/fonts/debug.ttf");
// 	printf(debugFont->isLoaded() ? "loaded\n" : "not loaded\n");

	m_frameCount++;
	m_frameTime += delta;
	if (m_frameTime > 0.5)
	{
		m_fps = m_frameCount / m_frameTime;
		m_frameTime = 0;
		m_frameCount = 0;
	}

	GLint viewportRect[4];
	glGetIntegerv(GL_VIEWPORT, viewportRect);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(	viewportRect[0], viewportRect[2],
				viewportRect[1], viewportRect[3]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);
	m_font.printf(Vector2f(10, 10), "%04.2f FPS", getFPS());

	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();

	return Renderer::run(delta);
}
