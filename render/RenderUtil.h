//////////////////////////////////////////////////////////////////////////
//
// This file is part of Shinzui.
//
// Shinzui is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shinzui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shinzui.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////
//
// RenderUtil.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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
