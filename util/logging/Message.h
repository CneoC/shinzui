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
// Message.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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
