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
// Color4.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __MATH_COLOR4_H__
#define __MATH_COLOR4_H__

#include "Math.h"

namespace math
{
	/**
	 * Color RGBA class.
	 */
	template <typename T>
	class Color4
	{
	public:
		Color4()
#ifdef _DEBUG
			: r(InvalidValue<T>::value)
			, g(InvalidValue<T>::value)
			, b(InvalidValue<T>::value)
			, a(InvalidValue<T>::value)
#endif
		{
		}

		Color4(T _r, T _g, T _b, T _a)
			: r(_r)
			, g(_g)
			, b(_b)
			, a(_a)
		{
		}

		Color4(const Color4 &copy)
			: r(copy.r)
			, g(copy.g)
			, b(copy.b)
			, a(copy.a)
		{
		}

	public:
		T r, g, b, a;
	};

	typedef Color4<int>		Color4i;
	typedef Color4<u8>		Color4b;
	typedef Color4<float>	Color4f;
}

#endif //__MATH_COLOR4_H__
