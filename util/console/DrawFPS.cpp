#include "DrawFPS.h"

#include "core/Core.h"

#include "render/RenderDriver.h"
#include "render/2d/FontUtil.h"

#include "resources/ResourceLoader.h"
#include "resources/ResourceCache.h"
#include "resources/font/GLFontResource.h"
#include "resources/font/FTFontResource.h"

#include <gl/glew.h>

using namespace console;

//////////////////////////////////////////////////////////////////////////

DrawFPS::DrawFPS(Core *pCore)
	: render::Renderer(pCore)
	, m_frameCount(0)
	, m_frameTime(0)
	, m_fps(0)
{
	m_pFontUtil = m_pCore->getDriver()->getUtil("2d.Font")->as<render::FontUtil>();

	FTFontResource ftFont(m_pCore->getLoader(), "File::2d/fonts/debug.ttf", "fps");
	ftFont->setSize(10, 128);

	m_font = GLFontResource(ftFont);
	m_font.load();
}

DrawFPS::~DrawFPS()
{
}

void DrawFPS::render(double delta)
{
	m_pFontUtil->setResource(m_font);

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
	m_pFontUtil->printf(math::Vector2f(10, 10), "%04.2f FPS", getFPS());

	ResourceCache *pCache = m_pCore->getLoader()->getCache();
	if (pCache->isLoading())
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		m_pFontUtil->printf(math::Vector2f(800, 10), "Loading... %3.1f%%", pCache->getProgress() * 100);
	}

	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}
