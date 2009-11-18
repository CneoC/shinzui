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
// ResourceLoader.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RESOURCE_RESOURCELOADER_H__
#define __RESOURCE_RESOURCELOADER_H__

#include "ResourceLoaderBase.h"

#include "util/logging/Log.h"

//////////////////////////////////////////////////////////////////////////

namespace resources
{
	class ResourceCache;

	/**
	 * Root class for resource loaders.
	 * Uses a ResourceCache instance to cache and stream loaded resources.
	 */
	class ResourceLoader
		: public ResourceLoaderBase
	{
	public:
		/**
		 * Constructs a base resource loader.
		 * @param pCache	the ResourceCache to use to manage cached and async loaded resources.
		 */
		ResourceLoader(ResourceCache *pCache = NULL);

		//////////////////////////////////////////////////////////////////////////

		//! @see ResourceLoaderBase::get
		virtual Resource get(const ResourceId &id);
		//! @see ResourceLoaderBase::convert
		virtual Resource convert(const Resource &resource, const ResourceType &type);
		//! @see ResourceLoaderBase::clone
		virtual Resource clone(const Resource &src, Resource dst);

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Loads a resource (blocking).
		 * @return if resource was properly loaded.
		 */
		virtual bool load(Resource &res, u32 flags = FLAG_NONE)		{ return false; }

		/**
		 * Unloads a resource (blocking).
		 */
		virtual bool unload(Resource &res, u32 flags = FLAG_NONE)	{ return false; }

		//////////////////////////////////////////////////////////////////////////

		//! Gets the resource cache
		ResourceCache *getCache() const		{ return m_pCache; }

	protected:
		ResourceCache *	m_pCache;
		logging::Log *	m_log;
	};
}

#endif //__RESOURCE_RESOURCELOADER_H__
