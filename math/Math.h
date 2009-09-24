#pragma once

#ifndef __MATH_H__
#define __MATH_H__

class Math
{
public:
	/**
	 * Calculate next power of two
	 */
	template <typename T>
	static T nextPower2(T v)
	{
		T p = 1;
		while (p < v)
		{
			p <<= 1;
		}
		return p;
	}

};

#endif //__MATH_H__
