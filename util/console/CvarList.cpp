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
// CvarList.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "CvarList.h"

#include "Cvar.h"

using namespace console;

void CvarList::add(CvarDef *pCvar)
{
	assert(m_variables.find(pCvar->getHash()) == m_variables.end());	// Only unique hashes supported atm
	m_variables.insert(std::make_pair(pCvar->getHash(), pCvar));
}

void CvarList::remove(CvarDef *pCvar)
{
	m_variables.erase(pCvar->getHash());
}

bool CvarList::has(u32 hash) const
{
	return m_variables.find(hash) != m_variables.end();
}

CvarDef *CvarList::find(u32 hash)
{
	assert(has(hash));
	return m_variables.find(hash)->second;
}