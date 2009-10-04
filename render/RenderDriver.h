#pragma once

#ifndef __RENDER_RENDERDRIVER_H__
#define __RENDER_RENDERDRIVER_H__

#include "RendererLoaderBase.h"

namespace os
{
	class ContextBase;
}

namespace render
{
	class RenderDriver
		: public RendererLoaderBase
	{
	protected:
		//! Don't allow external construction.
		RenderDriver(core::Core *pCore)
			: RendererLoaderBase(pCore)
			, m_pContext(NULL)
			, m_pLoaderContext(NULL)
		{}

	public:
		//! Gets the context.
		os::ContextBase *getContext() const			{ return m_pContext; }
		//! Gets the context for the loader thread(s).
		os::ContextBase *getLoaderContext() const	{ return m_pLoaderContext; }

	protected:
		os::ContextBase *m_pContext;
		os::ContextBase *m_pLoaderContext;
	};
}

#endif //__RENDER_RENDERDRIVER_H__
