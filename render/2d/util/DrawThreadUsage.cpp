//////////////////////////////////////////////////////////////////////////
//
// This file is part of Shinzui.
//
// Shinzui is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shinzui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shinzui.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////
//
// DrawThreadUsage.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "DrawThreadUsage.h"

#include "util/console/ThreadUsage.h"

#include "os/current/gl/GLContext.h"

using namespace render;
using namespace console;

//////////////////////////////////////////////////////////////////////////

DrawThreadUsage::DrawThreadUsage(core::Core *pCore, const ThreadUsage &usage, double showDuration)
	: Renderer(pCore)
	, m_usage(usage)
	, m_showDuration(showDuration)
{
}

DrawThreadUsage::~DrawThreadUsage()
{
}

void DrawThreadUsage::render(double delta)
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

	float width = (float)(rec[2] - rec[0] - 10);
	float offsetX = 5;
	glTranslatef(offsetX, (float)(rec[3] - rec[1] - 5), 0.0f);

	for (u32 i = 0; i < m_pCore->getThreadCount(); i++)
	{
		const ThreadUsage::ActivityInfo &info = m_usage.getActivityInfo(i);
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

		ThreadUsage::ActivityInfo::ActivityList::const_iterator iter;
		for (iter = info.m_list.begin(); iter != info.m_list.end(); ++iter)
		{
			float startX	= (float)(width * (1 - ((start - iter->m_start) / m_showDuration)));
			if (startX < offsetX)
				startX = offsetX;
			float endX		= (float)(width * (1 - ((start - iter->m_end) / m_showDuration)));
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

		if (info.m_current.m_pProcess)
		{
			float startX	= (float)(width * (1 - ((start - info.m_current.m_start) / m_showDuration)));
			if (startX < offsetX)
				startX = offsetX;

			math::Color3f c = info.m_current.m_pProcess->getColor();
			glVertex3f(startX, -1.0f, 0.0f);
			glVertex3f(startX,  1.0f, 0.0f);
			glVertex3f(width,  1.0f, 0.0f);
			glVertex3f(width, -1.0f, 0.0f);
		}
		glEnd();
		glTranslatef(0.0f, -3.0f, 0.0f);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}
