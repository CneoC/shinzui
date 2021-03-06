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
// Writer.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __LOGGING_WRITER_H__
#define __LOGGING_WRITER_H__

#include "core/types.h"

#include <boost/thread.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>

namespace fs = boost::filesystem;

namespace logging
{
	class Formatter;
	class Message;
	class Level;

	/**
	 * Log writer base class, responsible for writing actual messages to a target.
	 */
	class Writer
	{
	public:
		Writer()
			: m_pFormatter(NULL)
			, m_pLevel(NULL)
		{}

		virtual void open() = 0;
		virtual void close() = 0;

		virtual void write(Message &message) = 0;

		virtual const char *getName() = 0;

		void setFormatter(Formatter *pFormatter)	{ m_pFormatter = pFormatter; }
		Formatter *getFormatter() const				{ return m_pFormatter; }

		const Level *getLevel() const				{ return m_pLevel; }
		void setLevel(const Level &level)			{ m_pLevel = &level; }

	protected:
		Formatter *		m_pFormatter;
		const Level *	m_pLevel;
	};

	/**
	 * Writer that outputs messages to the command prompt
	 */
	class ConsoleWriter
		: public Writer
	{
	public:
		virtual void open() {}
		virtual void close() {}

		virtual void write(Message &message);

		virtual const char *getName() { return "ConsoleWriter"; }

	protected:
		static boost::mutex ms_mutex;
	};

	/**
	 * Writer that outputs messages to a file
	 */
	class FileWriter
		: public Writer
	{
	public:
		FileWriter(const fs::path &path)
			: m_path(path)
			, m_openCount(0)
		{}

		virtual void open();
		virtual void close();

		virtual void write(Message &message);

		virtual const char *getName() { return "FileWriter"; }

	protected:
		static boost::mutex ms_mutex;

		u32					m_openCount;
		std::ofstream		m_file;
		fs::path			m_path;
	};

} //namespace logging

#endif //__LOGGING_WRITER_H__
