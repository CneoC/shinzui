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
