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

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLint rec[4];
	glGetIntegerv(GL_VIEWPORT, rec);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(rec[0], rec[2], rec[3], rec[1], -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float width = rec[2] - rec[0] - 10;
	glTranslatef(5, rec[3] - rec[1] - 5, 0.0f);

	for (u32 i = 0; i < m_pCore->getThreadCount(); i++)
	{
		ThreadUsageInfo::ActivityInfo &info = m_info.getActivityInfo(i);
		boost::shared_lock<boost::shared_mutex> lock(info.m_mutex);

		double start = os::Time(os::Time::NOW).getSeconds();

		glBegin(GL_QUADS);
			if (i & 1)
				glColor4f(0.1f, 0.1f, 0.1f, 0.2f);
			else
				glColor4f(0.2f, 0.2f, 0.2f, 0.4f);
			glVertex3f( 5, -1.0f, 0.0f);
			glVertex3f( 5,  1.0f, 0.0f);
			glVertex3f( width,  1.0f, 0.0f);
			glVertex3f( width, -1.0f, 0.0f);

			ThreadUsageInfo::ActivityInfo::ActivityList::const_iterator iter;
			for (iter = info.m_list.begin(); iter != info.m_list.end(); ++iter)
			{
				float startX	= (float)(width * (1 - ((start - iter->m_start) / m_info.getShowDuration())));
				float endX		= (float)(width * (1 - ((start - iter->m_end) / m_info.getShowDuration())));
				if (startX < 0)
					startX = 0;

				math::Color3f c = iter->m_pProcess->getColor();
				glColor4f(c.r, c.g, c.b, 0.5f);
				glVertex3f(startX, -1.0f, 0.0f);
				glVertex3f(startX,  1.0f, 0.0f);
				glVertex3f(endX,  1.0f, 0.0f);
				glVertex3f(endX, -1.0f, 0.0f);

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
