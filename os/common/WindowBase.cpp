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
// WindowBase.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "WindowBase.h"

using namespace os;

//////////////////////////////////////////////////////////////////////////

WindowBase::WindowBase(core::Core *pCore)
	: Process(pCore, 0)
	, m_size(800, 600)
	, m_fullscreen(false)
	, m_bpp(0)
{
	setFrameDelay(0.1);
	m_color = math::Color3f(0, 0, 1);
}

WindowBase::~WindowBase()
{
}
