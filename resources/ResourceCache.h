#pragma once

#ifndef __RESOURCE_CACHE_H__
#define __RESOURCE_CACHE_H__

class ResourceCache
{
protected:
	struct Entry
	{
		Time		lastUsed;
		Resource	resource;
	};

public:
	Resource get(const char *id)
	{
		u32 hash = generateHash(id);
		Entry &result = m_resources[hash];
		result.lastUsed = Time(Time::NOW);
		return result.resource;
	}

	void put(const char *id, const Resource &resource)
	{
		u32 hash = generateHash(id);
		Entry &entry = m_resources[hash];
		entry.lastUsed = Time(Time::NOW);
		entry.resource = resource;
	}

	void clean()
	{

	}

	void clear()
	{
		m_resources.clear();
	}

protected:
	u32 generateHash(const char *c) const
	{
		u32 hash = 0;

		while (c)
		{
			hash = *c + (hash << 6) + (hash << 16) - hash;
			++c;
		}
		return hash;
	}

	std::map<u32, Entry>	m_resources;
};

#endif //__RESOURCE_CACHE_H__
