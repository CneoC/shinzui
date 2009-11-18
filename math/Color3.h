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
// Color3.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __MATH_COLOR3_H__
#define __MATH_COLOR3_H__

#include "Math.h"

namespace math
{
	/**
	 * Color RGB class.
	 */
	template <typename T>
	class Color3
	{
	public:
		Color3()
#ifdef _DEBUG
			: r(InvalidValue<T>::value)
			, g(InvalidValue<T>::value)
			, b(InvalidValue<T>::value)
#endif
		{
		}

		Color3(T _r, T _g, T _b)
			: r(_r)
			, g(_g)
			, b(_b)
		{
		}

		Color3(const Color3 &copy)
			: r(copy.r)
			, g(copy.g)
			, b(copy.b)
		{
		}

	public:
		T r, g, b;
	};

	typedef Color3<int>		Color3i;
	typedef Color3<u8>		Color3b;
	typedef Color3<float>	Color3f;
}

#endif //__MATH_COLOR3_H__
