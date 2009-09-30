#include "RendererLoaderBase.h"

#include "RenderUtil.h"

using namespace render;

//////////////////////////////////////////////////////////////////////////

RendererLoaderBase::~RendererLoaderBase()
{
	// Clean all utils
	UtilList::iterator i = m_utils.begin();
	while (i != m_utils.end())
	{
		delete i->second;
		++i;
	}
}

//////////////////////////////////////////////////////////////////////////

Renderer *RendererLoaderBase::createRenderer(u32 hash) const
{
	// Find the hash
	RendererList::const_iterator found = m_renderers.find(hash);
	if (found != m_renderers.end())
	{
		return found->second(m_pCore);
	}
	// Look in child loaders for proper renderer
	else
	{
		LoaderList::const_iterator i = m_loaders.begin();
		while (i != m_loaders.end())
		{
			Renderer *pRenderer = (*i)->createRenderer(hash);
			if (pRenderer)
				return pRenderer;
			++i;
		}
	}
}

RenderUtil *RendererLoaderBase::getUtil(u32 hash) const
{
	// Find the hash
	UtilList::const_iterator found = m_utils.find(hash);
	if (found != m_utils.end())
	{
		return found->second;
	}
	// Look in child loaders for proper util
	else
	{
		LoaderList::const_iterator i = m_loaders.begin();
		while (i != m_loaders.end())
		{
			RenderUtil *pUtil = (*i)->getUtil(hash);
			if (pUtil)
				return pUtil;
			++i;
		}
	}
	return NULL;
}
