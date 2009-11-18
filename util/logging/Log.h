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
// Log.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __LOGGING_LOG_H__
#define __LOGGING_LOG_H__

#include "core/types.h"

#include "Level.h"
#include "Message.h"

#include <list>
#include <sstream>

namespace logging
{
	class LogManager;
	class Writer;
	
	class Log
	{
	protected:
		friend class LogManager;

		/**
		 * Constructor used for root log.
		 */
		Log();

		//! Prohibit copying
		Log(const Log &) {}

	public:
		typedef std::list<Writer *>	WriterList;

		//////////////////////////////////////////////////////////////////////////

		Log(const char *pName);
		Log(const char *pName, const Level &level);
		~Log();

		//////////////////////////////////////////////////////////////////////////

		const char *getName() const				{ return m_pName; }
		
		const Level &getLevelRecursive() const	{ return m_pLevel? *m_pLevel: m_pParent->getLevelRecursive(); }

		const Level *getLevel() const			{ return m_pLevel; }
		void setLevel(const Level &level)		{ m_pLevel = &level; }

		Log *getParent() const					{ return m_pParent; }

		void addWriter(Writer *pWriter);
		void clearWriters();

		bool isRecurseWrite() const				{ return m_recurseWrite; }
		void setRecurseWrite(bool recurse)		{ m_recurseWrite = recurse; }

		//////////////////////////////////////////////////////////////////////////

		void write(Message &message);

	protected:
		void setParent(Log *pParent)			{ m_pParent = pParent; }

	protected:
		const char *	m_pName;
		const Level *	m_pLevel;

		bool			m_recurseWrite;
		WriterList		m_writers;

		Log *			m_pParent;
	};
}

#endif //__LOGGING_LOG_H__
