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
// ResourceId.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "ResourceId.h"

using namespace resources;

//////////////////////////////////////////////////////////////////////////

bool ResourceId::operator == (const ResourceId &other) const
{
	return	getHash() == other.getHash() &&
		m_type == other.getType() &&
		m_name == other.getName();
}

std::string ResourceId::toString() const
{
	if (m_type.isEmpty())
	{
		if (m_name.empty())
			return "<unnamed>";
		else
			return m_name;
	}
	return m_type.getTop() + "::" + m_name;
}

//////////////////////////////////////////////////////////////////////////

u32 ResourceId::getHash() const
{
	if (m_hash == 0)
	{
		m_hash = util::hashString(m_type.toString().c_str(), m_type.toString().length());
		m_hash = util::hashString(m_name.c_str(), m_name.length(), m_hash);
	}
	return m_hash;
}

//////////////////////////////////////////////////////////////////////////

void ResourceId::parseId(const std::string &id)
{
	u32 pos = id.find("::");
	if (pos != std::string::npos)
	{
		m_type = id.substr(0, pos);
		m_name = id.substr(pos + 2);
	}
	else
	{
		m_name = id;
	}
}