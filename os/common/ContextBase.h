#pragma once

#ifndef __OS_CONTEXTBASE_H__
#define __OS_CONTEXTBASE_H__

#include "os/current/Window.h"

namespace os
{
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

		virtual bool create() = 0;
		virtual bool destroy() = 0;

		virtual bool bind() = 0;
		virtual bool unbind() = 0;
		virtual bool link(ContextBase *pOther) = 0;

		virtual void update() = 0;

		virtual void swapBuffers() = 0;

		//////////////////////////////////////////////////////////////////////////

		Window *getWindow() const	{ return m_pWindow; }

	protected:
		void onResize(const math::Vector2i &size) { m_flags[FLAG_RESIZED] = true; }

	protected:
		std::bitset<16>	m_flags;
		Window *		m_pWindow;
	};
}

#endif //__OS_CONTEXTBASE_H__
