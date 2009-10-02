#pragma once

#ifndef __RENDER_RENDERER_H__
#define __RENDER_RENDERER_H__

#include "core/types.h"

#include "render/RenderDriver.h"
#include "os/interface/ContextBase.h"

namespace render
{
	/**
	 * Base class for a renderer.
	 */
	class Renderer
	{
	protected:
		//! Don't allow external construction.
		Renderer(Core *pCore)
			: m_pCore(pCore)
		{}

	public:
		/**
		 * Initialize renderer.
		 */
		virtual void init() {}

		/**
		 * Renders this renderer.
		 * @param delta	time delta since last render
		 */
		virtual void render(double delta) = 0;

		/**
		 * Cast to different renderer.
		 */
		template <class T>
		T *as()	{ return static_cast<T *>(this); }

	protected:
		//! Get context.
		ContextBase *getContext() const			{ return m_pCore->getDriver()->getContext(); }
		ContextBase *getLoaderContext() const	{ return m_pCore->getDriver()->getLoaderContext(); }

	protected:
		Core *		m_pCore;	// core class.
	};
}

#endif //__RENDER_RENDERER_H__
