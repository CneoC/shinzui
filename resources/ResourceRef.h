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
// ResourceRef.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCE_RESOURCEREF_H__
#define __RESOURCE_RESOURCEREF_H__

#include "core/types.h"

#include "resources/ResourceId.h"
#include "resources/ResourceType.h"

#include <assert.h>

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class ResourceLoaderBase;
	class ResourceData;

	template <class T> class ResourceRef;
	typedef ResourceRef<ResourceData>	Resource;

	enum DontConvertResource { DONT_CONVERT };

	//////////////////////////////////////////////////////////////////////////

	/**
	 * Reference counter for resource data objects.
	 * @template T	Type of resource data
	 */
	template <class T>
	class ResourceRef
	{
	public:
		/**
		 * Construct a NULL resource
		 */
		ResourceRef()
			: m_pData(NULL)
		{
		}

		/**
		 * Creates a custom user resource without any data.
		 * @param pLoader	The resource loader to use
		 */
		ResourceRef(ResourceLoaderBase *pLoader)
			: m_pData(new T(pLoader))
		{
			reference();
		}

		/**
		 * Loads a resource by name
		 * @param pLoader	The resource loader to use
		 * @param id		The identifier to get.
		 */
		ResourceRef(ResourceLoaderBase *pLoader, const ResourceId &id)
			: m_pData(NULL)
		{
			*this = pLoader->get(id);
		}

		/**
		 * Loads a resource by name and clones it with a new name.
		 * The resource data is not copied, just the definition, so it needs to be reloaded.
		 * @param pLoader	The resource loader to use
		 * @param id		The identifier to get.
		 * @param copy		If the resource was found make a copy of it with this identifier.
		 */
		ResourceRef(ResourceLoaderBase *pLoader, const ResourceId &id, const ResourceId &copy)
			: m_pData(NULL)
		{
			assert(copy);
			ResourceRef<T> src = pLoader->get(id);

			// If resource was loaded clone it
			if (src)
			{
				// Determine new id
				ResourceId id;
				if (copy.getType().isEmpty())
					id = ResourceId(src->getType().getTop(), copy.getName());
				else
					id = copy;

				// Create data with proper id
				m_pData = new T(src->getLoader());
				m_pData->setId(id);
				reference();

				// Let loader handle the rest of the cloning
				*this = pLoader->clone(src, *this);
			}
		}

		/**
		 * Construct resource with a resource data object.
		 * @param pData	Data object (ResourceRef takes ownership and will delete it once no references remain!)
		 */
		ResourceRef(T *pData)
			: m_pData(pData)
		{
			assert(m_pData);
			reference();
		}

		/**
		 * Copy constructor from different type of resource.
		 */
		template <class OT>
		ResourceRef(const ResourceRef<OT>& cast)
		{
			// Cast compatible resource types
			if (cast && cast.getType() & T::getName())
 			{
				// C-style cast works better here, due to it's dynamic nature it will
				// better determine cast types depending on T and OT template types.
				// A reinterpret_cast will break when data objects have virtual tables,
				// and static_cast won't work on different typed resource conversions
				// (even though resource conversions never end up in here).
 				m_pData = (T *)cast.getData();
 				reference();
 			}
			// Try to convert incompatible resource types
			else if (cast)
			{
				ResourceRef<T> resource = cast->getLoader()->getRoot()->convert(cast, ResourceType(T::getName()));
				m_pData = resource.getData();
				if (m_pData) 
					reference();
			}
			// Otherwise become a NULL resource 
			else
			{
				m_pData = NULL;
			}
		}

		/**
		 * Conversion constructor from different type of resource.
		 */
		template <class OT>
		ResourceRef(const ResourceRef<OT>& cast, DontConvertResource)
		{
			// Cast compatible resource types
			if (cast && cast.getType() & T::getName())
			{
				m_pData = static_cast<T *>(cast.getData());
				reference();
			}
			// Otherwise become a NULL resource 
			else
			{
				m_pData = NULL;
			}
		}

		/**
		 * Copy constructor.
		 */
		ResourceRef(const ResourceRef& copy)
			: m_pData(copy.m_pData)
		{
			reference();
		}

		/**
		 * Destructor
		 */
		~ResourceRef()
		{
			dereference();
		}

		//////////////////////////////////////////////////////////////////////////

		ResourceRef &operator = (const ResourceRef &copy)
		{
			if (m_pData != copy.m_pData)
			{
				m_pData = copy.m_pData;
				reference();
			}
			return *this;
		}

		bool operator < (const ResourceRef &other) const
		{
			if (!other || !*this)
				return false;
			// Higher priority is more important so should come earlier
			return (*this)->getLoadPriority() > other->getLoadPriority();
		}

		T *operator -> ()				{ return m_pData; }
		const T *operator -> () const	{ return m_pData; }

		operator bool () const			{ return m_pData != NULL; }

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Load the resource
		 * @param flags		see ResourceLoaderBase::LoadFlags
		 */
		bool load(u32 flags = 0)
		{
			return m_pData && m_pData->getLoader()->load(ResourceRef<ResourceData>(*this, DONT_CONVERT), flags);
		}

		/**
		* Unload the resource
		* @param flags		see ResourceLoaderBase::LoadFlags
		*/
		bool unload(u32 flags = 0)
		{
			return m_pData && m_pData->getLoader()->unload(ResourceRef<ResourceData>(*this, DONT_CONVERT), flags);
		}

		/**
		 * Reload the resource
		 * @param flags		see ResourceLoaderBase::LoadFlags
		 */
		bool reload(u32 flags = 0)
		{
			return unload(flags) && load(flags);
		}

		//! Safe check to see if the data is loaded.
		bool isLoaded() const	{ return m_pData && m_pData->isLoaded(); }

		//////////////////////////////////////////////////////////////////////////

		ResourceType getType() const
		{
			if (m_pData)
				return m_pData->getType();
			return ResourceType();
		}

		T *getData() const				{ return m_pData; }

	protected:
		void reference()
		{
			if (m_pData)
				m_pData->incRefCount();
		}

		void dereference()
		{
			if (m_pData)
			{
				m_pData->decRefCount();
				if (m_pData->getRefCount() == 0)
					delete m_pData;
			}
		}

	protected:
		T*	m_pData;
	};
}

#endif //__RESOURCE_RESOURCEREF_H__
