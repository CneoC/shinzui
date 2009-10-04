#pragma once

#ifndef __OS_TIME_H__
#define __OS_TIME_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>

namespace os
{
	class Time
	{
	public:
		// Initial value
		enum NowValue	{ NOW };

		Time()
		{
			m_value.QuadPart = 0;
			m_frequency.QuadPart = 0;
		}

		Time(NowValue)
		{
			QueryPerformanceCounter(&m_value);
			QueryPerformanceFrequency(&m_frequency);
		}

		Time(const Time &copy)
		{
			m_value.QuadPart = copy.m_value.QuadPart;
			m_frequency.QuadPart = copy.m_frequency.QuadPart;
		}

		//////////////////////////////////////////////////////////////////////////

		double getSeconds() const
		{
			assert(m_frequency.QuadPart > 0);
			return (double)m_value.QuadPart / (double)m_frequency.QuadPart;
		}

		//////////////////////////////////////////////////////////////////////////

		Time &operator -= (const Time &other)
		{
			m_value.QuadPart = m_value.QuadPart - other.m_value.QuadPart;
			return *this;
		}

		Time operator - (const Time &other) const
		{
			Time result(*this);
			result -= other;
			return result;
		}

		Time &operator += (const Time &other)
		{
			m_value.QuadPart = m_value.QuadPart + other.m_value.QuadPart;
			return *this;
		}

		Time operator + (const Time &other) const
		{
			Time result(*this);
			result += other;
			return result;
		}

		//////////////////////////////////////////////////////////////////////////

		bool operator == (const Time &other) const
		{
			return m_value.QuadPart == m_value.QuadPart + other.m_value.QuadPart;
		}

		bool operator < (const Time &other) const
		{
			return m_value.QuadPart < m_value.QuadPart + other.m_value.QuadPart;
		}

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
