#pragma once

#ifndef __TIME_H__
#define __TIME_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>

class Time
{
public:
	// Initial value
	enum NoneValue	{ NONE };
	enum NowValue	{ NOW };

	Time(NoneValue = NONE)
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
		: m_value(copy.m_value)
		, m_frequency(copy.m_frequency)
	{
	}

	double getSeconds()
	{
		assert(m_frequency.QuadPart > 0);
		return (double)m_value.QuadPart / (double)m_frequency.QuadPart;
	}

	Time operator - (const Time &other)
	{
		Time result;
		result.m_value.QuadPart = m_value.QuadPart - other.m_value.QuadPart;
		result.m_frequency = m_frequency;
		return result;
	}

	Time operator + (const Time &other)
	{
		Time result;
		result.m_value.QuadPart = m_value.QuadPart + other.m_value.QuadPart;
		result.m_frequency = m_frequency;
		return result;
	}

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

#endif //__TIME_H__
