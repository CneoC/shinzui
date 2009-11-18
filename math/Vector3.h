#pragma once

#ifndef __MATH_VECTOR3_H__
#define __MATH_VECTOR3_H__

#include "Math.h"

namespace math
{
	/**
	 * 3D Vector class.
	 */
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

		//////////////////////////////////////////////////////////////////////////

		Vector3 &operator += (const Vector3 &other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		Vector3 operator + (const Vector3 &other) const
		{
			Vector3 result(*this);
			return result += other;
		}

		Vector3 &operator -= (const Vector3 &other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		Vector3 operator - (const Vector3 &other) const
		{
			Vector3 result(*this);
			return result -= other;
		}

		Vector3 &operator *= (const Vector3 &other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}
		Vector3 operator * (const Vector3 &other) const
		{
			Vector3 result(*this);
			return result *= other;
		}

		Vector3 &operator /= (const Vector3 &other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}
		Vector3 operator / (const Vector3 &other) const
		{
			Vector3 result(*this);
			return result /= other;
		}

		Vector3 &operator *= (T other)
		{
			x *= other;
			y *= other;
			z *= other;
			return *this;
		}
		Vector3 operator * (T other) const
		{
			Vector3 result(*this);
			return result *= other;
		}

		Vector3 &operator /= (T other)
		{
			x /= other;
			y /= other;
			z /= other;
			return *this;
		}
		Vector3 operator / (T other) const
		{
			Vector3 result(*this);
			return result /= other;
		}

		Vector3 operator - () const
		{
			return Vector3(-x, -y, -z);
		}


		//////////////////////////////////////////////////////////////////////////

		T getDotProduct() const
		{
			return x * x + y * y + z * z;
		}

		T getSquaredDist() const
		{
			Vector3 result(*this);
			return getDotProduct();
		}

		T getDist() const
		{
			return sqrt(getSquaredDist());
		}

		Vector3 &normalize(T &magnitude)
		{
			magnitude = getDist();
			T invMagnitude = 1.0 / magnitude;
			x *= invMagnitude;
			y *= invMagnitude;
			z *= invMagnitude;
			return *this;
		}

		Vector3 getNormalized(T &magnitude) const
		{
			return Vector3(*this).normalize(magnitude);
		}

		Vector3 &normalize()
		{
			T magnitude;
			return normalize(magnitude);
		}

		Vector3 getNormalized() const
		{
			T magnitude;
			return Vector3(*this).normalize(magnitude);
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
