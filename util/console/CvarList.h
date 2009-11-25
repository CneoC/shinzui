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
// CvarList.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __CONSOLE_CVAR_LIST_H__
#define __CONSOLE_CVAR_LIST_H__

#include "core/types.h"
#include "util/Util.h"

#include <map>

namespace console
{
	class CvarDef;

	class CvarList
	{
	public:
		//! A map of Cvars indexed by hash.
		typedef std::map<u32, CvarDef *>	CvarMap;

		//////////////////////////////////////////////////////////////////////////
		
		/**
		 * Global cvar list.
		 * Contains all registered cvars in the engine (and game) for easy lookup.
		 */
		static CvarList &instance()	{ static CvarList inst; return inst; }

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Adds a cvar to the group.
		 * @param cvar	The cvar to add.
		 */
		void add(CvarDef *pCvar);

		/**
		 * Removes a cvar from the group.
		 * @param cvar	The cvar to remove.
		 */
		void remove(CvarDef *pCvar);

		/**
		 * Checks if the group contains a cvar with a specific hash.
		 * @param hash	The hash to check for an attached cvar.
		 */
		bool has(u32 hash) const;

		/**
		 * Finds a cvar value by it's hash.
		 * @param hash The hash of the cvar we're looking for.
		 */
		CvarDef *find(u32 hash);
		
		/**
		 * Finds a cvar value by it's name
		 * @param name	The name of the cvar we're looking for.
		 */
		CvarDef *find(const std::string &name)	{ return find(util::hashString(name)); }

	protected:
		CvarMap		m_variables;	//!< The list of cvar variables registered in this group.
									//!< All Cvar hashes are expected to be unique in the current implementation.
	};
}

#endif /* __CONSOLE_CVAR_LIST_H__ */