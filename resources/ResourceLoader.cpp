#include "ResourceLoader.h"

#include "ResourceCache.h"

ResourceLoader::ResourceLoader(ResourceCache *pCache)
	: m_pCache(pCache)
	, m_log(LOG_GET("Resource.Loader"))
{
}

Resource ResourceLoader::get(const ResourceId &id)
{
	LOG_INFO(m_log, '\'' << id.toString() << '\'');

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
	LOG_INFO(m_log, '\'' << resource->getId().getType().getTop() << "::" << resource->getId().getName() << "' to type " << type.toString());

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
	LOG_INFO(m_log, '\'' << src->getId().getType().getTop() << "::" << src->getId().getName() << "' -> '" << dst->getId().getType().getTop() << "::" << dst->getId().getName());

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
