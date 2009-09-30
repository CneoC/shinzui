#pragma once

#ifndef __RESOURCE_CACHE_H__
#define __RESOURCE_CACHE_H__

#include "core/Process.h"

#include "resources/Resource.h"

#include "util/Util.h"

#include <map>
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>

class ResourceCache
	: public Process
{
public:
	struct Entry
	{
		typedef std::list<Resource>	List;
		List	resources;
	};

	// TODO: use actual hash map
	typedef std::map<u32, Entry>	ResourceList;

public:
	/**
	 * Constructs a resource cache process.
	 * @param pCore				Core class.
	 * @param id				Process identifier for lookups.
	 * @param targetThreadId	Target thread id to run this process on (THREAD_ID_NORMAL_MASK for any thread)
	 */
	ResourceCache(Core *pCore, int id = 0, int targetThreadId = Core::THREAD_ID_NORMAL_MASK)
		: Process(pCore, id, targetThreadId)
		, m_log(LOG_GET("Resources.Cache"))
	{
	}

	/**
	 * Finds a resource with a specific identifier and type
	 */
	Resource find(const ResourceId &id)
	{
		LOG_INFO(m_log, LOG_FMT("'%s'", id.toString()));

		// early exit
		if (m_resources.empty()) return Resource();

		boost::shared_lock<boost::shared_mutex> lock(m_resourcesMutex);

		// calculate string hash
		u32 hash = id.getHash();

		// find hash in resource map
		ResourceList::iterator found = m_resources.find(hash);
		if (found != m_resources.end())
		{
			// for all resources with the same hash key
			// these can be different conversions of a resource 
			// or duplicate hashes when ALLOW_HASH_COLLISIONS
			Entry::List::iterator iter = found->second.resources.begin();
			while (iter != found->second.resources.end())
			{
				Resource resource = *iter;

				// check if it's actually the resource we're looking for
				if (resource->getId() == id)
					return *iter;

				++iter;
			}
		}
		return Resource();
	}

	void add(const Resource &resource)
	{
		LOG_INFO(m_log, LOG_FMT("'%s' with type %s", resource->getId().toString() % resource->getType().toString()));

		assert(resource);
		boost::lock_guard<boost::shared_mutex> lock(m_resourcesMutex);

		// calculate string hash
		u32 hash = resource->getId().getHash();

		// find hash in resource map
		Entry &entry = m_resources[hash];
		entry.resources.push_back(resource);
	}

	void clean()
	{
//		boost::lock_guard<boost::shared_mutex> lock(m_resourcesMutex);
	}

	void clear()
	{
		boost::lock_guard<boost::shared_mutex> lock(m_resourcesMutex);

		m_resources.clear();
	}

	void load()
	{
		boost::shared_lock<boost::shared_mutex> lock(m_resourcesMutex);

		ResourceList::iterator i;
		for (i = m_resources.begin(); i != m_resources.end(); ++i)
		{
			Entry::List::iterator j;
			for (j = i->second.resources.begin(); j != i->second.resources.end(); ++j)
			{
				Resource resource = *j;
				if (resource->isLoading())
					resource.load(0);
			}
		}
	}

	void unload()
	{
		boost::shared_lock<boost::shared_mutex> lock(m_resourcesMutex);

		ResourceList::iterator i;
		for (i = m_resources.begin(); i != m_resources.end(); ++i)
		{
			Entry::List::iterator j;
			for (j = i->second.resources.begin(); j != i->second.resources.end(); ++j)
			{
				Resource resource = *j;
				if (resource->isUnloading())
					resource.unload(0);
			}
		}
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
	ResourceList		m_resources;
	logging::Log *		m_log;
};

#endif //__RESOURCE_CACHE_H__
