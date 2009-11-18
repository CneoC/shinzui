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
// Math.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __MATH_H__
#define __MATH_H__

#include "core/types.h"

#include <cmath>

namespace math
{
	/**
	 * Calculate next power of two
	 */
	template <typename T>
	inline T getNextPowerOfTwo(T v)
	{
		T p = 1;
		while (p < v)
		{
			p <<= 1;
		}
		return p;
	}

	/**
	 * Checks if an integer is a power of two.
	 */
	inline bool isPowerOfTwo(u32 v) { return !(v & (v - 1)) && v; }

	template <typename T>
	struct InvalidValue
	{
		static const T value;
	};
	template <typename T> const T InvalidValue<T>::value = static_cast<T>(0xC0DEF00D);
}

#endif //__MATH_H__
