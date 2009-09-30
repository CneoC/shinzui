#pragma once

#ifndef __RENDER_RENDERDRIVER_H__
#define __RENDER_RENDERDRIVER_H__

#include "RendererLoaderBase.h"

class ContextBase;

namespace render
{
	class RenderDriver
		: public RendererLoaderBase
	{
	protected:
		//! Don't allow external construction.
		RenderDriver(Core *pCore)
			: RendererLoaderBase(pCore)
			, m_pContext(NULL)
		{}

	public:
		//! Gets the context.
		ContextBase *getContext() const	{ return m_pContext; }

	protected:
		ContextBase *m_pContext;
	};
}

#endif //__RENDER_RENDERDRIVER_H__
