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
// ResourceCache.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "ResourceCache.h"

using namespace resources;

//////////////////////////////////////////////////////////////////////////

ResourceCache::ResourceCache(core::Core *pCore, int id)
	: core::Process(pCore, id)
	, m_log(LOG_GET("Resources.Cache"))
	, m_maxLoadingCount(0)
{
	m_color = math::Color3f(0, 1, 0);
	m_pCore->addJob(this, core::Job::Function(this, &ResourceCache::initJob), core::Core::THREAD_ID_LOAD_BIT);
}

Resource ResourceCache::find(const ResourceId &id)
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

void ResourceCache::add(Resource resource)
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
	setDelay(0);
}

//////////////////////////////////////////////////////////////////////////

void ResourceCache::onStart()
{
	// TODO: Move these to a separate job?
	update();
	unload();

	// Initialize before we start loading
// 	if (getLastRunTime() == 0)
// 	{
// 		m_pCore->addJob(this, core::Job::Function(this, &ResourceCache::initJob), core::Core::THREAD_ID_LOAD_BIT);
// 	}
// 	else
// 	{
	// Create load jobs
	if (!m_load.empty())
	{
		m_pCore->addJob(this, core::Job::Function(this, &ResourceCache::loadCoreJob), core::Core::THREAD_ID_LOAD_BIT);
		for (u32 i = 1; i < getLoadJobs(); i++)
		{
			m_pCore->addJob(this, core::Job::Function(this, &ResourceCache::loadJob));
		}

		setDelay(0);
	}
	// Delay execution of this process if we have nothing to load
	else
	{
		setDelay(1);
	}
// 	}
}

//////////////////////////////////////////////////////////////////////////

void ResourceCache::update()
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

void ResourceCache::load(bool coreJob)
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
				if (getLoadJobs() == 1 || (coreJob == (*i)->requiresContext()))
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

void ResourceCache::unload()
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
