#pragma once

#ifndef __RESOURCE_CACHE_H__
#define __RESOURCE_CACHE_H__

#include "util/Util.h"

class ResourceCache
	: public Process
{
public:
	typedef std::map<u32, Resource>	ResourceMap;

	/**
	 * Constructs a resource cache process.
	 * @param pCore			core class.
	 * @param id				process identifier for lookups.
	 * @param targetThreadId	target thread id to run this process on (THREAD_ID_NONE for any thread)
	 */
	ResourceCache(Core *pCore, int id = 0, int targetThreadId = Core::THREAD_ID_NORMAL)
		: Process(pCore, id, targetThreadId)
	{
	}

	Resource get(const std::string &id)
	{
		u32 hash = Util::hashString(id.c_str(), id.length());
		return m_resources[hash];
	}

	void put(const std::string &id, const Resource &resource)
	{
		u32 hash = Util::hashString(id.c_str(), id.length());
		m_resources[hash] = resource;
	}

	void clean()
	{

	}

	void clear()
	{
		m_resources.clear();
	}

	virtual Process* run(double delta)
	{
		clean();
		return this;
	}

protected:
	ResourceMap	m_resources;
};

#endif //__RESOURCE_CACHE_H__
