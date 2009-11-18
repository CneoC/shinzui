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
// Vector2.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __MATH_VECTOR2_H__
#define __MATH_VECTOR2_H__

#include "Math.h"

namespace math
{
	/**
	 * 2D Vector class.
	 */
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
