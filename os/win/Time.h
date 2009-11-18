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
// Time.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __OS_TIME_H__
#define __OS_TIME_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>

namespace os
{
	/**
	 * High detail time class.
	 */
	class Time
	{
	public:
		// Initial value
		enum NowValue	{ NOW };

		/**
		 * Initializes time to zero.
		 */
		Time()
		{
			m_value.QuadPart = 0;
			m_frequency.QuadPart = 0;
		}

		/**
		 * Initializes time to NOW.
		 */
		Time(NowValue)
		{
			QueryPerformanceCounter(&m_value);
			QueryPerformanceFrequency(&m_frequency);
		}

		/**
		 * Copy constructor.
		 */
		Time(const Time &copy)
		{
			m_value.QuadPart = copy.m_value.QuadPart;
			m_frequency.QuadPart = copy.m_frequency.QuadPart;
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Gets the number of seconds as a double.
		 * Possibly causes some precision loss.
		 */
		double getSeconds() const
		{
			assert(m_frequency.QuadPart > 0);
			return (double)m_value.QuadPart / (double)m_frequency.QuadPart;
		}

		//////////////////////////////////////////////////////////////////////////

		//! Subtract two times.
		Time &operator -= (const Time &other)
		{
			m_value.QuadPart = m_value.QuadPart - other.m_value.QuadPart;
			return *this;
		}
		//! Subtract two times.
		Time operator - (const Time &other) const
		{
			Time result(*this);
			result -= other;
			return result;
		}

		//! Add two times.
		Time &operator += (const Time &other)
		{
			m_value.QuadPart = m_value.QuadPart + other.m_value.QuadPart;
			return *this;
		}
		//! Add two times.
		Time operator + (const Time &other) const
		{
			Time result(*this);
			result += other;
			return result;
		}

		//////////////////////////////////////////////////////////////////////////

		//! Compare if two times are equal.
		bool operator == (const Time &other) const
		{
			return m_value.QuadPart == m_value.QuadPart + other.m_value.QuadPart;
		}

		//! Compare if this time comes earlier than the other time.
		bool operator < (const Time &other) const
		{
			return m_value.QuadPart < m_value.QuadPart + other.m_value.QuadPart;
		}

		//! Compare if this time comes later than the other time.
		bool operator > (const Time &other) const
		{
			return m_value.QuadPart > m_value.QuadPart + other.m_value.QuadPart;
		}

	protected:
		LARGE_INTEGER m_value;
		LARGE_INTEGER m_frequency;
	};
}

#endif //__OS_TIME_H__
