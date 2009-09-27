#pragma once

#ifndef __LOGGING_MESSAGE_H__
#define __LOGGING_MESSAGE_H__

#include <string>

namespace logging
{
	class Log;
	class Level;

	/**
	 * Class that represents a location in the code where a message was sent from.
	 */
	class Location
	{
	public:
		Location(const char *pFile, u32 line, const char *pFunction)
			: m_pFile(pFile)
			, m_line(line)
			, m_pFunction(pFunction)
		{}

		const char *getFile() const		{ return m_pFile; }
		u32			getLine() const		{ return m_line; }
		const char *getFunction() const	{ return m_pFunction; }

	protected:
		const char *m_pFile;
		u32			m_line;
		const char *m_pFunction;
	};

	/**
	 * Class that contains information about a single log message.
	 */
	class Message
	{
	public:
		Message(Log &log, const Level &level, const Location &location, const std::string &message)
			: m_log(log)
			, m_level(level)
			, m_location(location)
			, m_message(message)
		{}

		Log &getLog() const						{ return m_log; }
		const Level &getLevel() const			{ return m_level; }
		const Location &getLocation() const		{ return m_location; }
		const std::string &getMessage() const	{ return m_message; }

	protected:
		Log &				m_log;
		const Level &		m_level;
		const Location &	m_location;
		const std::string &	m_message;
	};

} // namespace logging

#endif //__LOGGING_MESSAGE_H__
