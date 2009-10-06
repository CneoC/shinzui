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
