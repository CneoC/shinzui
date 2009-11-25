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
// Console.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __CONSOLE_CONSOLE_H__
#define __CONSOLE_CONSOLE_H__

#include "core/Process.h"

#include "script/PythonScriptEngine.h"

#include "util/logging/Writer.h"

#include "CvarList.h"

namespace console
{
	/**
	 * Class that represents a single line of console text.
	 */
	class ConsoleLine
	{
	public:
		ConsoleLine(const std::string &text)
			: m_text(text)
		{}

		const std::string &getText() const	{ return m_text; }

	protected:
		std::string m_text;
	};

	/**
	 * Console class.
	 */
	class Console
		: public core::Process
	{
	public:
		typedef std::list<ConsoleLine>	ConsoleBuffer;

		Console(core::Core *pCore, u32 id = 0);
		~Console();

		//////////////////////////////////////////////////////////////////////////
		
		virtual void onStart();

		//////////////////////////////////////////////////////////////////////////
		
		const ConsoleBuffer &getBuffer() const	{ return m_buffer; }

		//////////////////////////////////////////////////////////////////////////
		
		/**
		 * Clears the console buffer.
		 */
		void clear()							{ m_buffer.clear(); }

		/**
		 * Writes one or more lines of text to the console.
		 * @param text	The text to append to the console.
		 */
		void write(const std::string &text);

		/**
		 * Writes a single line of text to the console.
		 * @param text	The line to append to the console.
		 */
		void writeLine(const std::string &line);

	protected:
		ConsoleBuffer				m_buffer;
		script::PythonScriptEngine	m_scriptEngine;

		boost::mutex				m_bufferMutex;
	};


	/**
	 * Log writer that outputs messages to the in-game console.
	 */
	class GameConsoleWriter
		: public logging::Writer
	{
	public:
		GameConsoleWriter(Console &console)
			: m_console(console)
		{}

		virtual void open() {}
		virtual void close() {}

		virtual void write(logging::Message &message);

		virtual const char *getName() { return "GameConsoleWriter"; }

	protected:
		Console &			m_console;
	};
}

#endif //__CONSOLE_CONSOLE_H__
