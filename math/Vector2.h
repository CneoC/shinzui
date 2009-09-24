#pragma once

#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include <cmath>

template <typename T>
class Vector2
{
public:
	Vector2()
#ifdef _DEBUG
		: x(FLT_MAX)
		, y(FLT_MAX)
#endif
	{
	}

	Vector2(T _x, T _y)
		: x(_x)
		, y(_y)
	{
	}

	Vector2(Vector2 &copy)
		: x(copy.x)
		, y(copy.y)
	{
	}


	T x;
	T y;
};

typedef Vector2<int>	Vector2i;
typedef Vector2<short>	Vector2s;
typedef Vector2<float>	Vector2f;

#endif //__VECTOR2_H__