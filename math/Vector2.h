#pragma once

#ifndef __MATH_VECTOR2_H__
#define __MATH_VECTOR2_H__

#include "Math.h"

namespace math
{
	template <typename T>
	class Vector2
	{
	public:
		Vector2()
	#ifdef _DEBUG
			: x(InvalidValue<T>::value)
			, y(InvalidValue<T>::value)
	#endif
		{
		}

		Vector2(T _x, T _y)
			: x(_x)
			, y(_y)
		{
		}

		Vector2(const Vector2 &copy)
			: x(copy.x)
			, y(copy.y)
		{
		}

	public:
		T x, y;
	};

	typedef Vector2<int>	Vector2i;
	typedef Vector2<short>	Vector2s;
	typedef Vector2<float>	Vector2f;
}

#endif //__MATH_VECTOR2_H__
