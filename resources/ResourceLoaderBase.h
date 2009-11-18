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
// ResourceLoaderBase.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCES_RESOURCELOADERBASE_H__
#define __RESOURCES_RESOURCELOADERBASE_H__

#include "Resource.h"

#include <list>

#include <boost/shared_ptr.hpp>

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	/**
	 * Base class for resource loaders.
	 */
	class ResourceLoaderBase
	{
	protected:
		// Don't allow construction.
		ResourceLoaderBase()
			: m_pParent(NULL)
		{}

	public:
		// Shared ResourceLoaderBase pointer for proper cleanup.
		// shared_ptr is used because ResourceLoaderBase instances
		// are allowed to be used in multiple resource loader trees.
		typedef boost::shared_ptr<ResourceLoaderBase>	SharedPtr;
		typedef std::list<SharedPtr>					LoaderList;

		enum LoadFlags
		{
			FLAG_NONE,
			FLAG_ASYNC				= 1 << 0,		// Mark the (un)load as asynchronous
			FLAG_DONT_RECURSE		= 1 << 1,		// Mark the (un)load as none recursive (Resource::getSource())

			ASYNC_PRIORITY_MASK		= 0xFF000000,	// Mask async priority value
			ASYNC_PRIORITY_SHIFT	= 24,			// Shift async priority value

			// Asynchronous load/unload priority
			ASYNC_PRIORITY_CRITICAL	= 255 << ASYNC_PRIORITY_SHIFT,
			ASYNC_PRIORITY_HIGH		= 128 << ASYNC_PRIORITY_SHIFT,
			ASYNC_PRIORITY_NORMAL	= 64 << ASYNC_PRIORITY_SHIFT,
			ASYNC_PRIORITY_LOW		= 1 << ASYNC_PRIORITY_SHIFT,

			FLAG_LOAD_DEFAULT		= FLAG_NONE,
			FLAG_UNLOAD_DEFAULT		= FLAG_DONT_RECURSE
		};

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Gets a resource object by name (possibly not yet loaded).
		 * @param id		Identifier of the resource.
		 * @param rename	Copies the resource if valid and gives it this new id.
		 * @return The requested resource (a NULL resource when unable to load).
		 */
		virtual Resource get(const ResourceId &id);

		/**
		 * Converts a resource from one type to another if possible.
		 * @param resource	The resource to convert.
		 * @param type		The type to convert the resource to.
		 */
		virtual Resource convert(const Resource &res, const ResourceType &type);

		/**
		 * Clones an existing resource.
		 * This is a very limited implementation and most loaders will need to
		 * implement their own versions.
		 * @param src	Source resource to clone.
		 * @param dst	Destination resource to clone to.
		 */
		virtual Resource clone(const Resource &src, Resource dst);

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Loads a resource (blocking).
		 * @return if resource was properly loaded.
		 */
		virtual bool load(Resource &res, u32 flags = FLAG_LOAD_DEFAULT);

		/**
		 * Unloads a resource (blocking).
		 */
		virtual bool unload(Resource &res, u32 flags = FLAG_UNLOAD_DEFAULT);

		//////////////////////////////////////////////////////////////////////////

		//! Registers a child loader to manage more specific loading.
		void addLoader(const SharedPtr &loader)		{ m_loaders.push_front(loader); loader->setParent(this); }
		//! Registers a child loader and puts it in a ResourceLoaderRef to manage more specific loading.
		void addLoader(ResourceLoaderBase *pLoader)	{ addLoader(SharedPtr(pLoader)); }
		//! Removes a child loader.
		void removeLoader(const SharedPtr &loader)	{ m_loaders.remove(loader); }
		//! Clear all child loaders
		void clearLoaders()							{ m_loaders.clear(); }

		//////////////////////////////////////////////////////////////////////////

		//! Gets this loaders parent loader.
		ResourceLoaderBase *getParent() const		{ return m_pParent; }
		//! Returns the root loader of the current loader tree.
		ResourceLoaderBase *getRoot()				{ return m_pParent? m_pParent->getRoot(): this; }

	protected:
		//! Sets this loaders parent loader.
		void setParent(ResourceLoaderBase *pParent) { m_pParent = pParent; }

	protected:
		ResourceLoaderBase *m_pParent;
		LoaderList			m_loaders;
	};
}

#endif //__RESOURCES_RESOURCELOADERBASE_H__
