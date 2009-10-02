#pragma once

#ifndef __RENDER_RENDERUTIL_H__
#define __RENDER_RENDERUTIL_H__

namespace render
{
	/**
	 * Base class for simple render related utilities.
	 */
	class RenderUtil
	{
	protected:
		//! Don't allow construction.
		RenderUtil() {}

	public:
		/**
		 * Cast to different utility.
		 */
		template <class T>
		T *as()	{ return static_cast<T *>(this); }
	};
}

#endif //__RENDER_RENDERUTIL_H__
