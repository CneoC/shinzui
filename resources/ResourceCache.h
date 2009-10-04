#pragma once

#ifndef __RESOURCE_CACHE_H__
#define __RESOURCE_CACHE_H__

#include "core/Process.h"

#include "resources/Resource.h"

#include "render/RenderDriver.h"
#include "os/common//ContextBase.h"

#include "util/Util.h"

#include <map>
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>

class ResourceCache
	: public core::Process
{
public:
	struct Entry
	{
		typedef std::list<Resource>	List;
		List	resources;
	};

	// TODO: use actual hash map
	typedef std::map<u32, Entry>	ResourceMap;
	typedef std::list<Resource>		ResourceList;

public:
	/**
	 * Constructs a resource cache process.
	 * @param pCore				Core class.
	 * @param id				Process identifier for lookups.
	 * @param threadMask	Target thread id to run this process on (THREAD_ID_NORMAL_MASK for any thread)
	 */
	ResourceCache(core::Core *pCore, int id = 0)
		: Process(pCore, id, core::Core::THREAD_ID_LOAD_BIT, core::Core::THREAD_ID_NORMAL_MASK)
		, m_log(LOG_GET("Resources.Cache"))
		, m_maxLoadingCount(0)
	{
		m_color = math::Color3f(0, 1, 0);
	}

	/**
	 * Finds a resource with a specific identifier and type
	 */
	Resource find(const ResourceId &id)
	{
		LOG_TRACE(m_log, LOG_FMT("'%s'", id.toString()));

		// early exit
		if (m_resources.empty()) return Resource();

		boost::shared_lock<boost::shared_mutex> lock(m_resourcesMutex);

		// calculate string hash
		u32 hash = id.getHash();

		// find hash in resource map
		ResourceMap::iterator found = m_resources.find(hash);
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

	void add(Resource resource)
	{
		LOG_TRACE(m_log, LOG_FMT("'%s' with type %s", resource->getId().toString() % resource->getType().toString()));

		assert(resource);

		// calculate string hash
		u32 hash = resource->getId().getHash();

		{
			boost::lock_guard<boost::shared_mutex> lock(m_resourcesMutex);

			// find hash in resource map
			Entry &entry = m_resources[hash];
			entry.resources.push_back(resource);
		}

		// Maybe there's something to load now
		setFrameDelay(0);
	}

	void update()
	{
		{
			boost::lock_guard<boost::shared_mutex> lock(m_resourcesMutex);

			bool loadAdded = false;
			// For all resources
			ResourceMap::iterator i;
			for (i = m_resources.begin(); i != m_resources.end(); ++i)
			{
				// And all non-unique hashes
				Entry::List::iterator j;
				for (j = i->second.resources.begin(); j != i->second.resources.end();)
				{
					Resource resource = *j;
					Entry::List::iterator prev = j;
					++j;

					// If the resources is marked for unloading, put it in the unload queue.
					if (resource->isUnloading()) // TODO: Time out if unused (and NO_TIMEOUT flag not set)
					{
						resource->setUnload(false);
						m_unload.push_back(resource);
						i->second.resources.erase(prev);
						break;
					}
					// If the resource is marked for loading, put it in the load queue.
					// if resource is (still) loaded wait for unload to complete (to support reload)
					else if (resource->isLoading() && !resource->isLoaded())
					{
						resource->setLoad(false);
						m_load.push_front(resource);
						loadAdded = true;
					}
				}
			}

			if (loadAdded)
				m_load.sort();
		}
	}

	void clear()
	{
		boost::lock_guard<boost::shared_mutex> lock(m_resourcesMutex);
		m_resources.clear();
	}

	void load(bool coreJob)
	{
		// Resource we want to load
		Resource loadRes;
		{
			// Shared lock on m_resources
			boost::upgrade_lock<boost::shared_mutex> lock(m_resourcesMutex);

			// If there is a resource to load
			if (!m_load.empty())
			{
				// Find a resource that needs loading
				ResourceList::iterator i = m_load.begin();
				while (i != m_load.end())
				{
					// Only load a resource in the core job if we have one job or
					// if the resource requires the loading context.
					if (getJobs() == 1 || !coreJob || (*i)->requiresContext())
					{
						boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
						loadRes = *i;
						m_load.erase(i);
						break;
					}
					++i;
				}
			}
		}

		u32 loads = m_load.size();
		m_maxLoadingCount = loads? max(m_maxLoadingCount, loads): 0;

		// Load one of the resources
		// This is done separately because of the m_resourcesMutex lock 
		// since add() will likely get called from inside the load.
		loadRes.load(0);
	}

	void unload(bool coreJob)
	{
		// If there is a resource to unload
		if (!m_unload.empty())
		{
			// Resource we want to unload
			Resource unloadRes;
			{
				// Shared lock on m_resources
				boost::shared_lock<boost::shared_mutex> lock(m_resourcesMutex);

				// Find a resource that needs unloading.
				unloadRes = m_unload.back();
				m_unload.pop_back();
			}

			unloadRes.unload(0);
		}
	}

	virtual void init()
	{
		m_pCore->getDriver()->getLoaderContext()->bind();
	}

	virtual core::Process *run(u32 job, double delta)
	{
		bool coreJob = job == 0;

		// Only update on first job
		if (coreJob)
		{
			update();

			if (m_load.empty())
				setFrameDelay(1);
			else
				setFrameDelay(0);
		}

		unload(coreJob);
		load(coreJob);

		return this;
	}

	//////////////////////////////////////////////////////////////////////////

	bool isLoading() const		{ return !m_load.empty(); }
	float getProgress() const	{ return m_maxLoadingCount > 0? min(1.0f, 1 - (float)m_load.size() / m_maxLoadingCount): 1.0f; }

protected:
	boost::shared_mutex	m_resourcesMutex;		// mutex used to lock list of processes.
	ResourceMap			m_resources;

	ResourceList		m_load;
	ResourceList		m_unload;
	
	logging::Log *		m_log;

	volatile float		m_maxLoadingCount;
};

#endif //__RESOURCE_CACHE_H__
