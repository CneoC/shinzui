#pragma once

#ifndef __MATH_VECTOR3_H__
#define __MATH_VECTOR3_H__

#include "Math.h"

namespace math
{
	template <typename T>
	class Vector3
	{
	public:
		Vector3()
	#ifdef _DEBUG
			: x(InvalidValue<T>::value)
			, y(InvalidValue<T>::value)
			, z(InvalidValue<T>::value)
	#endif
		{
		}

		Vector3(T _x, T _y, T _z)
			: x(_x)
			, y(_y)
			, z(_z)
		{
		}

		Vector3(const Vector3 &copy)
			: x(copy.x)
			, y(copy.y)
			, z(copy.z)
		{
		}

	public:
		T x, y, z;
	};

	typedef Vector3<int>	Vector3i;
	typedef Vector3<short>	Vector3s;
	typedef Vector3<float>	Vector3f;
	typedef Vector3<double>	Vector3d;
}

#endif //__MATH_VECTOR3_H__
