#pragma once

#ifndef __OS_CONTEXTBASE_H__
#define __OS_CONTEXTBASE_H__

#include "os/current/Window.h"

namespace os
{
	/**
	 * Base class for rendering contexts.
	 */
	class ContextBase
	{
	public:
		enum Flags
		{
			FLAG_RESIZED
		};

		//////////////////////////////////////////////////////////////////////////

		ContextBase(Window *pWindow)
			: m_pWindow(pWindow) 
		{
			pWindow->resizeEvent.connect(this, &ContextBase::onResize);
		}

		//////////////////////////////////////////////////////////////////////////

		//! Creates the context.
		virtual bool create() = 0;
		//! Destroys the context.
		virtual bool destroy() = 0;

		//! Binds the context to the current thread.
		virtual bool bind() = 0;
		//! Unbinds the context from the current thread.
		virtual bool unbind() = 0;

		//! Links the resources of 2 contexts.
		virtual bool link(ContextBase *pOther) = 0;

		//! Updates the context.
		virtual void update() = 0;

		//! Swap the buffers for this context.
		virtual void swapBuffers() = 0;

		//////////////////////////////////////////////////////////////////////////

		//! Gets the window this context belongs to.
		Window *getWindow() const	{ return m_pWindow; }

	protected:
		//! Resize event
		void onResize(const math::Vector2i &size) { m_flags[FLAG_RESIZED] = true; }

	protected:
		std::bitset<16>	m_flags;
		Window *		m_pWindow;
	};
}

#endif //__OS_CONTEXTBASE_H__
