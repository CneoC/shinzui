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

	GLint viewportRect[4];
	glGetIntegerv(GL_VIEWPORT, viewportRect);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(	viewportRect[0], viewportRect[2],
				viewportRect[1], viewportRect[3]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(10.0f, 700.0f, 0.0f);

	double width = 500;
	for (u32 i = 0; i < m_pCore->getThreadCount(); i++)
	{
		ThreadUsageInfo::ActivityInfo &info = m_info.getActivityInfo(i);
		boost::lock_guard<boost::mutex> lock(info.m_mutex);

		double prevOffset = 0;
		glBegin(GL_QUADS);
			bool active = info.m_firstActive;
			ThreadUsageInfo::ActivityInfo::TimeList::const_iterator time;
			for (time = info.m_times.begin(); time != info.m_times.end(); ++time)
			{
				if (active)
					glColor3f(0.5f, 0.6f, 1.0f);
				else
					glColor3f(0.4f, 0.4f, 0.4f);

				double diff = m_info.getDuration() - (m_pCore->getElapsedTime() - *time);
				double offset = (diff / m_info.getDuration()) * width;

				glVertex3f( prevOffset,  2.0f, 0.0f);
				glVertex3f( prevOffset, -2.0f, 0.0f);
				glVertex3f( offset, -2.0f, 0.0f);
				glVertex3f( offset,  2.0f, 0.0f);

				prevOffset = offset;

				active = !active;
			}
		glEnd();
		glTranslatef(0.0f, -5.0f, 0.0f);

	}

	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}
