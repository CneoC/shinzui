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
// ResourceLoader.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "ResourceLoader.h"

#include "ResourceCache.h"

using namespace resources;

//////////////////////////////////////////////////////////////////////////

ResourceLoader::ResourceLoader(ResourceCache *pCache)
	: m_pCache(pCache)
	, m_log(LOG_GET("Resource.Loader"))
{
}

Resource ResourceLoader::get(const ResourceId &id)
{
	LOG_TRACE(m_log, '\'' << id.toString() << '\'');

	Resource result;

	// Find existing resources with the identifier
	if (m_pCache)
	{
		result = m_pCache->find(id);
	}

	// Haven't found a resource
	if (!result)
	{
		// Try to retrieve the resource
		result = ResourceLoaderBase::get(id);

		// Cache the retrieved resource
		if (m_pCache && result)
			m_pCache->add(result);
	}

	return result;
}

Resource ResourceLoader::convert(const Resource &resource, const ResourceType &type)
{
	LOG_TRACE(m_log, '\'' << resource->getId().getType().getTop() << "::" << resource->getId().getName() << "' to type " << type.toString());

	// Resource id we want to convert to
	ResourceId id(type.getTop() + "::" + resource->getId().getName());

	Resource result;
	// Find existing resources with the identifier
	if (m_pCache)
		result = m_pCache->find(id);
	
	// Haven't found a resource
	if (!result)
	{
		// Try to convert the resource
		result = ResourceLoaderBase::convert(resource, type);

		// Cache the converted resource
		if (m_pCache && result)
			m_pCache->add(result);
	}

	return result;
}

Resource ResourceLoader::clone(const Resource &src, Resource dst)
{
	LOG_TRACE(m_log, '\'' << src->getId().getType().getTop() << "::" << src->getId().getName() << "' -> '" << dst->getId().getType().getTop() << "::" << dst->getId().getName());

	Resource result;

	// Find existing resources with the identifier
	if (m_pCache)
	{
		result = m_pCache->find(dst->getId());
	}

	if (!result)
	{
		result = ResourceLoaderBase::clone(src, dst);

		// Cache the cloned resource
		if (m_pCache && result)
			m_pCache->add(result);
	}

	return result;
}
