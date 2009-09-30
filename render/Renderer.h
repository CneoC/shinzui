#pragma once

#ifndef __RENDER_RENDERER_H__
#define __RENDER_RENDERER_H__

class Core;

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
		 * Renders this renderer.
		 * @param delta	time delta since last render
		 */
		virtual void render(double delta) = 0;

		//! Gets the next renderer.
		Renderer *getNext() const		{ return m_pNext; }
		//! Sets the next renderer.
		void setNext(Renderer *pNext)	{ m_pNext = pNext; }

		/**
		 * Cast to different renderer.
		 */
		template <class T>
		T *as()	{ return static_cast<T *>(this); }

	protected:
		Core *		m_pCore;	// core class.
		Renderer *	m_pNext;	// next renderer in the chain.
	};
}

#endif //__RENDER_RENDERER_H__
