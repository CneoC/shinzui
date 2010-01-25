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
// DrawSWF.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "DrawSWF.h"

#include "core/Core.h"

#include "render/RenderDriver.h"

#include "resources/ResourceLoader.h"
#include "resources/ResourceCache.h"

#include "os/current/gl/GLContext.h"

#include "gameswf/gameswf.h"
#include <stdlib.h>
#include <stdio.h>
#include "gameswf/base/utility.h"
#include "gameswf/base/container.h"
#include "gameswf/base/tu_file.h"
#include "gameswf/base/tu_types.h"
#include "gameswf/base/tu_timer.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_freetype.h"
#include "gameswf/gameswf_player.h"

using namespace render;
using namespace resources;

//////////////////////////////////////////////////////////////////////////

static tu_file*	file_opener(const char* url)
// Callback function.  This opens files for the gameswf library.
{
	return new tu_file(url, "rb");
}

static void	fs_callback(gameswf::character* movie, const char* command, const char* args)
{
	assert(movie);
	gameswf::gc_ptr<gameswf::player> player = movie->get_player();
	if (stricmp(command, "fullscreen") == 0)
	{
		// TODO
	}
}

DrawSWF::DrawSWF(core::Core *pCore)
	: render::Renderer(pCore)
{
	gameswf::register_file_opener_callback(file_opener);
	gameswf::register_fscommand_callback(fs_callback);

	m_pPlayer = new gameswf::player();

	m_pRenderer = gameswf::create_render_handler_ogl();

	gameswf::set_render_handler(m_pRenderer);
	gameswf::set_glyph_provider(gameswf::create_glyph_provider_freetype());

	m_pRoot = m_pPlayer->load_file("data/test.swf");

	m_pRenderer->open();
	m_pRenderer->set_antialiased(true);

	m_pRoot = m_pPlayer->get_root();
}

DrawSWF::~DrawSWF()
{
	delete m_pRenderer;
}

void DrawSWF::render(double delta)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS); 
	GLint rec[4];
	glGetIntegerv(GL_VIEWPORT, rec);
 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, 1, -1, -1, 1);
 	
 	glMatrixMode(GL_MODELVIEW);
 	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//int width = m_pRoot->get_movie_width();
	//int height = m_pRoot->get_movie_height();
	m_pRoot->set_display_viewport(0, 0, rec[2], rec[3]);
	m_pRoot->set_background_alpha(0.1f);

	m_pRoot->advance((float)delta);
	m_pRoot->display();

	glPopAttrib();
}