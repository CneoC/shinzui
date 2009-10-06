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