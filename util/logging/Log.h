#pragma once

#ifndef __LOGGING_LOG_H__
#define __LOGGING_LOG_H__

#include "core/types.h"

#include "Level.h"
#include "Message.h"

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
		Log(const char *pName);
		Log(const char *pName, const Level &level);
		~Log();

		const char *getName() const				{ return m_pName; }
		
		const Level &getLevelRecursive() const	{ return m_pLevel? *m_pLevel: m_pParent->getLevelRecursive(); }

		const Level *getLevel() const			{ return m_pLevel; }
		void setLevel(const Level &level)		{ m_pLevel = &level; }

		Log *getParent() const					{ return m_pParent; }

		Writer *getWriter() const				{ return m_pWriter; }
		void setWriter(Writer *pWriter) 		{ m_pWriter = pWriter; }

		bool isRecurseWrite() const				{ return m_recurseWrite; }
		void setRecurseWrite(bool recurse)		{ m_recurseWrite = recurse; }

		void write(Message &message);

	protected:
		void setParent(Log *pParent)			{ m_pParent = pParent; }

	protected:
		const char *	m_pName;
		const Level *	m_pLevel;

		bool			m_recurseWrite;
		Writer *		m_pWriter;

		Log *			m_pParent;
	};
} //namespace log

#endif //__LOGGING_LOG_H__
