#include "ThreadUsage.h"

#include <gl/glew.h>

using namespace console;

//////////////////////////////////////////////////////////////////////////

ThreadUsage::ThreadUsage(core::Core *pCore)
	: Renderer(pCore)
	, m_info(pCore)
{
	m_info.start();
}

ThreadUsage::~ThreadUsage()
{
	m_info.stop();
}

void ThreadUsage::render(double delta)
{
	//printf("%s @ %f + %f\n", __FUNCTION__, getLastRunTime(), delta);

	glPushAttrib(GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_ENABLE_BIT); 

	GLint viewportRect[4];
	glGetIntegerv(GL_VIEWPORT, viewportRect);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(	viewportRect[0], viewportRect[2],
				viewportRect[1], viewportRect[3]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(10.0f, 760.0f, 0.0f);

	float width = 1000;
	for (u32 i = 0; i < m_pCore->getThreadCount(); i++)
	{
		ThreadUsageInfo::ActivityInfo &info = m_info.getActivityInfo(i);
		boost::shared_lock<boost::shared_mutex> lock(info.m_mutex);

		double start = os::Time(os::Time::NOW).getSeconds();

		glBegin(GL_QUADS);
			glColor3f(0.4, 0.4, 0.4);
			glVertex3f( 0,  1.0f, 0.0f);
			glVertex3f( 0, -1.0f, 0.0f);
			glVertex3f( width, -1.0f, 0.0f);
			glVertex3f( width,  1.0f, 0.0f);

			ThreadUsageInfo::ActivityInfo::ActivityList::const_iterator iter;
			for (iter = info.m_list.begin(); iter != info.m_list.end(); ++iter)
			{
				float startX	= width * (1 - ((start - iter->m_start) / m_info.getShowDuration()));
				float endX		= width * (1 - ((start - iter->m_end) / m_info.getShowDuration()));
				if (startX < 0)
					startX = 0;

				math::Color3f c = iter->m_pProcess->getColor();
				glColor3f(c.r, c.g, c.b);
				glVertex3f(startX,  1.0f, 0.0f);
				glVertex3f(startX, -1.0f, 0.0f);
				glVertex3f(endX, -1.0f, 0.0f);
				glVertex3f(endX,  1.0f, 0.0f);

				if (startX == 0)
					break;
			}
		glEnd();
		glTranslatef(0.0f, -3.0f, 0.0f);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}
