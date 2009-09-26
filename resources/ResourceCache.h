#pragma once

#ifndef __RESOURCE_CACHE_H__
#define __RESOURCE_CACHE_H__

#include "util/Util.h"
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>

class ResourceCache
	: public Process
{
public:
	typedef std::map<u32, Resource>	ResourceMap;

	/**
	 * Constructs a resource cache process.
	 * @param pCore			core class.
	 * @param id				process identifier for lookups.
	 * @param targetThreadId	target thread id to run this process on (THREAD_ID_NORMAL_MASK for any thread)
	 */
	ResourceCache(Core *pCore, int id = 0, int targetThreadId = Core::THREAD_ID_NORMAL_MASK)
		: Process(pCore, id, targetThreadId)
	{
	}

	Resource get(const std::string &id)
	{
		m_resourcesMutex.lock_shared();
		u32 hash = Util::hashString(id.c_str(), id.length());
		ResourceMap::iterator found = m_resources.find(hash);
		Resource result;
		if (found != m_resources.end()) result = found->second;
		m_resourcesMutex.unlock_shared();
		return result;
	}

	void put(const std::string &id, const Resource &resource)
	{
		assert(resource);
		m_resourcesMutex.lock();
		u32 hash = Util::hashString(id.c_str(), id.length());
		m_resources[hash] = resource;
		m_resourcesMutex.unlock();
	}

	void clean()
	{
		m_resourcesMutex.lock();
		m_resourcesMutex.unlock();
	}

	void clear()
	{
		m_resourcesMutex.lock();
		m_resources.clear();
		m_resourcesMutex.unlock();
	}

	void load()
	{
		m_resourcesMutex.lock_shared();
		ResourceMap::iterator iter;
		for (iter = m_resources.begin(); iter != m_resources.end(); ++iter)
		{
			if (iter->second->isLoading())
			{
				iter->second.load(0);
				iter->second->setLoad(false);
			}
		}
		m_resourcesMutex.unlock_shared();
	}

	void unload()
	{
		m_resourcesMutex.lock_shared();
		ResourceMap::iterator iter;
		for (iter = m_resources.begin(); iter != m_resources.end(); ++iter)
		{
			if (iter->second->isUnloading())
			{
				iter->second.unload(0);
				iter->second->setUnload(false);
			}
		}
		m_resourcesMutex.unlock_shared();
	}

	virtual Process *run(double delta)
	{
		clean();
		unload();
		load();
		return this;
	}

protected:
	boost::shared_mutex	m_resourcesMutex;		// mutex used to lock list of processes.
	ResourceMap			m_resources;
};

#endif //__RESOURCE_CACHE_H__
