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
// GLEndFrameBuffer.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "GLEndFrameBuffer.h"

#include "resources/ResourceLoaderBase.h"

#include "os/current/gl/GLContext.h"

#include <math.h>

using namespace render;

GLEndFrameBuffer::GLEndFrameBuffer(core::Core *pCore)
	: EndFrameBuffer(pCore)
{
}

GLEndFrameBuffer::~GLEndFrameBuffer()
{
}

void GLEndFrameBuffer::render(double delta)
{
	if (m_frameBuffer)
	{
		m_frameBuffer->unbind();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 1, 0, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (m_frameBuffer->getTexture().isLoaded())
		{
			glEnable(GL_TEXTURE_2D);
			m_frameBuffer->getTexture()->bind();
		}

		if (m_program.isLoaded())
			m_program->bind();

		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
			glTexCoord2i(0, 0); glVertex2f(0, 0);
			glTexCoord2i(1, 0); glVertex2f(1, 0);
			glTexCoord2i(1, 1); glVertex2f(1, 1);
			glTexCoord2i(0, 1); glVertex2f(0, 1);
		glEnd();

		if (m_program.isLoaded())
			m_program->unbind();

		if (m_frameBuffer->getTexture().isLoaded())
		{
			m_frameBuffer->getTexture()->unbind();
			glDisable(GL_TEXTURE_2D);
		}

		glPopMatrix();
	}
}
