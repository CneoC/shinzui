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
// Console.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __CONSOLE_CONSOLE_H__
#define __CONSOLE_CONSOLE_H__

#include "render/Renderer.h"

#include "CvarList.h"

#include <list>

namespace console
{
	class Console
		: public render::Renderer
	{
	public:
		typedef std::list< CvarList * >	CvarListList;

		Console(core::Core *pCore);
		~Console();

		virtual void render(double delta);

		void addCvarList(CvarList &group)		{ m_CvarLists.push_back(&group); }
		void removeCvarList(CvarList &group)	{ m_CvarLists.remove(&group); }

	protected:
		CvarListList	m_CvarLists;
	};
}

#endif //__CONSOLE_CONSOLE_H__
