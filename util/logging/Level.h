#pragma once

#ifndef __LOGGING_LEVEL_H__
#define __LOGGING_LEVEL_H__

namespace logging
{
	/**
	 * Class that defines a single log level type.
	 */
	class Level
	{
	public:
		/**
		 * Constructs a log level.
		 */
		Level(u32 level, const char *pName, const char *pNeatName, u32 indent)
			: m_level(level)
			, m_pName(pName)
			, m_pNeatName(pNeatName)
			, m_indent(indent)
		{
		}

		u32 getLevel() const			{ return m_level; }
		const char *getName() const		{ return m_pName; }
		const char *getNeatName() const	{ return m_pNeatName; }
		u32 getIndent() const			{ return m_indent; }

		bool operator < (const Level &other) const	{ return m_level < other.getLevel(); }
		bool operator <= (const Level &other) const	{ return m_level <= other.getLevel(); }
		bool operator > (const Level &other) const	{ return m_level > other.getLevel(); }
		bool operator >= (const Level &other) const	{ return m_level >= other.getLevel(); }
		bool operator == (const Level &other) const	{ return m_level == other.getLevel(); }

	protected:
		u32			m_level;
		const char *m_pName;
		const char *m_pNeatName;
		u32			m_indent;
	};

	/**
	 * Class that allows access to al log levels.
	 */
	class Levels
	{
	public:
	#define DEFINE_LEVEL(LEVEL, NAME, NEAT_NAME, INDENT) static Level &get##NEAT_NAME()	{ static Level level(LEVEL, #NAME, #NEAT_NAME, INDENT); return level; }
		DEFINE_LEVEL(0x6000,	LOG_FATAL,	Fatal,	0)
		DEFINE_LEVEL(0x5000,	LOG_ERROR,	Error,	0)
		DEFINE_LEVEL(0x4000,	LOG_WARN,	Warn,	0)
		DEFINE_LEVEL(0x3000,	LOG_INFO,	Info,	1)
		DEFINE_LEVEL(0x2000,	LOG_DEBUG,	Debug,	2)
		DEFINE_LEVEL(0x1000,	LOG_TRACE,	Trace,	3)
		DEFINE_LEVEL(0x0000,	LOG_ALL,	All,	0)
	#undef DEFINE_LEVEL
	};

	// Cleaner way to access levels
	#define LEVEL_FATAL	logging::Levels::getFatal()
	#define LEVEL_ERROR	logging::Levels::getError()
	#define LEVEL_WARN	logging::Levels::getWarn()
	#define LEVEL_INFO	logging::Levels::getInfo()
	#define LEVEL_DEBUG	logging::Levels::getDebug()
	#define LEVEL_TRACE	logging::Levels::getTrace()
#define LEVEL_ALL	logging::Levels::getAll()

}

#endif //__LOGGING_LEVEL_H__
