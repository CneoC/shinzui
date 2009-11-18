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
// ResourceData.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCE_RESOURCEDATA_H__
#define __RESOURCE_RESOURCEDATA_H__

#include "core/types.h"
#include "resources/ResourceId.h"
#include "resources/ResourceType.h"
#include "resources/ResourceLoaderBase.h"

#include "Resource.h"

#include "os/current/AtomicCounter.h"

#include <assert.h>
#include <bitset>
#include <boost/noncopyable.hpp>

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class ResourceData;

	template <class T> class ResourceRef;
	typedef ResourceRef<ResourceData>	Resource;

	//////////////////////////////////////////////////////////////////////////

	/**
	 * Base resource data class.
	 */
	class ResourceData
		: boost::noncopyable
	{
	public:
		enum Flags
		{
			FLAG_REQUIRE_CONTEXT,
			FLAG_LOADED,
			FLAG_LOAD,
			FLAG_UNLOAD,

			FLAG_NEXT
		};

		static const char *getName()	{ return "Resource"; }

		//////////////////////////////////////////////////////////////////////////

		ResourceData(ResourceLoaderBase *pLoader = NULL)
			: m_references(0)
			, m_pLoader(pLoader)
			, m_type(getName())
			, m_loadPriority(0)
		{
		}

		//////////////////////////////////////////////////////////////////////////

		//! Gets the resource identifier.
		const ResourceId &getId() const				{ return m_id; }
		//! Sets the resource identifier
		void setId(const ResourceId &id)			{ m_id = id; }

		//! Gets the resource type.
		const ResourceType &getType() const			{ return m_type; }
		ResourceType &getType()						{ return m_type; }

		//! Resets the resource flags.
		void resetFlags()							{ m_flags.reset(); }

		//! Resource is loaded.
		bool isLoaded() const						{ return m_flags[FLAG_LOADED]; }
		//! Set the resource is loaded.
		void setLoaded(bool loaded)					{ m_flags[FLAG_LOADED] = loaded; loaded? setLoad(false): setUnload(false); }

		//! Resource is marked for unloading.
		bool isLoading() const						{ return m_flags[FLAG_LOAD]; }
		//! Mark resource for (async) loading.
		void setLoad(bool load)						{ m_flags[FLAG_LOAD] = load; }

		//! Resource is marked for unloading.
		bool isUnloading() const					{ return m_flags[FLAG_UNLOAD]; }
		//! Mark resource for (async) unloading.
		void setUnload(bool unload)					{ m_flags[FLAG_UNLOAD] = unload; }

		//! Resource requires a context to be able to load
		bool requiresContext() const				{ return m_flags[FLAG_REQUIRE_CONTEXT]; }

		//! Gets the reference count for this resource data.
		u32 getRefCount() const						{ return m_references; }

		//! Increment resource data reference count (tread-safe).
		void incRefCount()							{ ++m_references; }
		//! Decrement resource data reference count (tread-safe).
		void decRefCount()							{  assert(m_references > 0); --m_references; }

		//! Gets the resource this one is converted from (can be a NULL resource).
		Resource getSource() const					{ return m_source; }
		//! Sets the resource this one is converted from.
		void setSource(const Resource &source)		{ m_source = source; }

		//! Get the loader responsible for this resource.
		ResourceLoaderBase *getLoader() const		{ return m_pLoader; }

		//! Gets the priority this resource has when loading asynchronous.
		s32 getLoadPriority() const					{ return m_loadPriority; }
		//! Sets the priority this resource has for asynchronous loading, higher is more important.
		void setLoadPriority(s32 priority)			{ m_loadPriority = (s16)priority; }

	protected:
		// TODO: Pack this data more optimally

		std::bitset<16>			m_flags;		// State flags.
		s16						m_loadPriority;

		ResourceId				m_id;			// Resource identifier.
		ResourceType			m_type;			// Resource types.

		os::AtomicCounter<u32>	m_references;	// Number of references to this resource.

		Resource				m_source;		// Source data for this resource.
		ResourceLoaderBase *	m_pLoader;		// Loader responsible for this resource.
	};
}

#endif //__RESOURCE_RESOURCEDATA_H__
