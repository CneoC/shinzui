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
// ResourceId.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCE_RESOURCEID_H__
#define __RESOURCE_RESOURCEID_H__

#include "util/Util.h"

#include "ResourceType.h"

#include <string>

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	/**
	 * Resource identifier class.
	 * Represents a resource by name and type combination.
	 */
	class ResourceId
	{
	public:
		//! Constructs a NULL ResourceId.
		ResourceId()
			: m_hash(0)
		{}

		//! Constructs a ResourceId from a string.
		ResourceId(const char *id)
			: m_hash(0)
		{
			parseId(id);
		}

		//! Constructs a ResourceId from string.
		ResourceId(const std::string &id)
			: m_hash(0)
		{
			parseId(id);
		}

		//! Constructs a ResourceId from a separate type and name.
		ResourceId(const std::string &type, const std::string &name)
			: m_hash(0)
			, m_type(type)
			, m_name(name)
		{
		}

		//////////////////////////////////////////////////////////////////////////

		//! Compares two resource id's.
		bool operator == (const ResourceId &other) const;

		//! Checks if the resource id is valid.
		operator bool () const	{ return !m_name.empty(); }

		/**
		 * Convert resource id to a string.
		 * @return the name of the identifier.
		 */
		std::string toString() const;

		//////////////////////////////////////////////////////////////////////////

		//! Gets the resource name.
		const std::string &getName() const	{ return m_name; }
		//! Gets the resource type.
		const ResourceType &getType() const	{ return m_type; }

		/**
		 * Gets the hash for this resource.
		 * @return hash including both name and type.
		 */
		u32 getHash() const;

	protected:
		/**
		 * Utility function to parse a string to a proper ResourceId.
		 */
		void parseId(const std::string &id);

	protected:
		mutable u32		m_hash;		// cached hash value.
		ResourceType	m_type;		// type of the resource.
		std::string		m_name;		// name of the resource.
	};
}

#endif //__RESOURCE_RESOURCEID_H__
