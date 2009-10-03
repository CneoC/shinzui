#pragma once

#ifndef __RESOURCE_RESOURCEID_H__
#define __RESOURCE_RESOURCEID_H__

#include "util/Util.h"

#include "ResourceType.h"

#include <string>

class ResourceId
{
public:
	ResourceId()
		: m_hash(0)
	{
	}

	ResourceId(const char *id)
		: m_hash(0)
	{
		parseId(id);
	}

	ResourceId(const std::string &id)
		: m_hash(0)
	{
		parseId(id);
	}

	ResourceId(const std::string &type, const std::string &name)
		: m_hash(0)
		, m_type(type)
		, m_name(name)
	{
	}

	//////////////////////////////////////////////////////////////////////////

	/**
	 * Compares two resource id's.
	 */
	bool operator == (const ResourceId &other) const
	{
		return	getHash() == other.getHash() &&
				m_type == other.getType() &&
				m_name == other.getName();
	}

	/**
	 * Checks if the resource id is valid.
	 */
	operator bool () const	{ return !m_name.empty(); }

	/**
	 * Convert resource id to a string.
	 * @return the name of the identifier.
	 */
	std::string toString() const
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

	//! Gets the resource name.
	const std::string &getName() const	{ return m_name; }
	//! Gets the resource type.
	const ResourceType &getType() const	{ return m_type; }

	/**
	 * Gets the hash for this resource.
	 * @return hash including both name and type.
	 */
	u32 getHash() const
	{
		if (m_hash == 0)
		{
			m_hash = util::hashString(m_type.toString().c_str(), m_type.toString().length());
			m_hash = util::hashString(m_name.c_str(), m_name.length(), m_hash);
		}
		return m_hash;
	}

protected:
	void parseId(const std::string &id)
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

protected:
	mutable u32		m_hash;		// Cached hash value
	ResourceType	m_type;
	std::string		m_name;
};

#endif //__RESOURCE_RESOURCEID_H__
