#pragma once

#ifndef __MATH_COLOR4_H__
#define __MATH_COLOR4_H__

#include "Math.h"

namespace math
{
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
