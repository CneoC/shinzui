#pragma once

#ifndef __MATH_COLOR3_H__
#define __MATH_COLOR3_H__

#include "Math.h"

namespace math
{
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
