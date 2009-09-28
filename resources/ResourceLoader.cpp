#include "ResourceLoader.h"

#include "ResourceCache.h"

ResourceLoader::ResourceLoader(ResourceCache *pCache)
	: m_pCache(pCache)
	, m_log(LOG_GET("Resource.Loader"))
{
}

Resource ResourceLoader::get(const std::string &id, ResourceType type)
{
	LOG_INFO(m_log, '\'' << id << '\'');

	Resource result;
	if (m_pCache)
	{
		result = m_pCache->find(id, type);
		if (result) return result;
	}
	result = ResourceLoaderBase::get(id, type);
	if (m_pCache && result)
		m_pCache->add(result);
	return result;
}

Resource ResourceLoader::convert(const Resource &resource, ResourceType type)
{
	LOG_INFO(m_log, '\'' << resource->getId() << "' to type " << type);

	Resource result;
	if (m_pCache)
	{
		result = m_pCache->find(resource->getId(), type);
		if (result) return result;
	}
	result = ResourceLoaderBase::convert(resource, type);
	if (m_pCache && result)
		m_pCache->add(result);
	return result;
}
