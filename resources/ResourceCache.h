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
// ResourceCache.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCE_CACHE_H__
#define __RESOURCE_CACHE_H__

#include "core/Process.h"

#include "resources/Resource.h"
#include "render/RenderDriver.h"

#include "os/common/ContextBase.h"

#include "util/Util.h"

#include <map>
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace resources
{
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
		 * @param pCore	Core class.
		 * @param id	Process identifier for lookups.
		 */
		ResourceCache(core::Core *pCore, int id = 0);

		/**
		 * Finds a resource with a specific identifier and type.
		 */
		Resource find(const ResourceId &id);

		void add(Resource resource);

		//////////////////////////////////////////////////////////////////////////

		virtual void onStart();

		//////////////////////////////////////////////////////////////////////////

		void setLoadJobs(u32 jobs)	{ m_loadJobs = jobs; }
		u32 getLoadJobs() const		{ return m_loadJobs; }

		bool isLoading() const		{ return !m_load.empty(); }
		float getProgress() const	{ return m_maxLoadingCount > 0? min(1.0f, 1 - (float)m_load.size() / m_maxLoadingCount): 1.0f; }

	protected:
		bool initJob()
		{
			// If we have a separate loader context
			if (getCore()->getDriver()->getLoaderContext())
			{
				getCore()->getDriver()->getLoaderContext()->bind();
			}
			return false;
		}

		bool loadCoreJob()
		{
			load(true);
			return !m_load.empty();
		}

		bool loadJob()
		{
			load(false);
			return !m_load.empty();
		}

		//////////////////////////////////////////////////////////////////////////

		void update();
		void load(bool coreJob);
		void unload();

		void clear()
		{
			boost::lock_guard<boost::shared_mutex> lock(m_resourcesMutex);
			m_resources.clear();
		}

	protected:
		boost::shared_mutex	m_resourcesMutex;		// mutex used to lock list of processes.
		ResourceMap			m_resources;

		ResourceList		m_load;
		ResourceList		m_unload;
		
		logging::Log *		m_log;

		u32					m_loadJobs;
		volatile float		m_maxLoadingCount;
	};
}

#endif //__RESOURCE_CACHE_H__
