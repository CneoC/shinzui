#include "RendererLoaderBase.h"

#include "RenderUtil.h"

using namespace render;

//////////////////////////////////////////////////////////////////////////

RendererLoaderBase::~RendererLoaderBase()
{
	// Clean all utilities
	UtilList::iterator util = m_util.begin();
	while (util != m_util.end())
	{
		delete util->second;
		++util;
	}

	// Clean all loaders
	LoaderList::iterator loader = m_loaders.begin();
	while (loader != m_loaders.end())
	{
		delete loader->second;
		++loader;
	}
}

//////////////////////////////////////////////////////////////////////////

Renderer *RendererLoaderBase::createRenderer(u32 hash) const
{
	// Find the hash
	RendererList::const_iterator found = m_renderers.find(hash);
	if (found != m_renderers.end())
		return found->second(m_pCore);
	return NULL;
}

RenderUtil *RendererLoaderBase::getUtil(u32 hash) const
{
	// Find the hash
	UtilList::const_iterator found = m_util.find(hash);
	if (found != m_util.end())
		return found->second;
	return NULL;
}

RendererLoaderBase *RendererLoaderBase::getLoader(const std::string &name) const
{
	u32 hash = util::hashString(name.c_str(), name.length());
	LoaderList::const_iterator found = m_loaders.find(hash);
	if (found != m_loaders.end())
		return found->second;
	return NULL;
}
