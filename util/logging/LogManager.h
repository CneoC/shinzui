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
// LogManager.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __UTIL_LOGGING_ROOT_H__
#define __UTIL_LOGGING_ROOT_H__

#include "Log.h"

#include <map>
#include <boost/format.hpp>

namespace logging
{
	/**
	 * Log manager which manages log instantiation and relations.
	 */
	class LogManager
	{
	public:
		typedef std::map<std::string, Log *> LogMap;

	protected:
		//! Singleton, protect constructor
		LogManager() {}

	public:
		void addLog(Log &log)
		{
			m_logs[log.getName()] = &log;

			// Find log parent
			const std::string name = log.getName();
			u32 pos = name.rfind('.');
			while (pos != std::string::npos)
			{
				std::string parent = name.substr(0, pos);
				LogMap::iterator find = m_logs.find(parent);
				if (find != m_logs.end())
				{
					log.setParent(find->second);
					return;
				}
				pos = name.rfind('.', pos - 1);
			}

			log.setParent(&m_rootLog);
		}

		void removeLog(Log &log)
		{
			LogMap::iterator iter = m_logs.find(log.getName());
			if (iter != m_logs.end())
				m_logs.erase(iter);
		}

		Log *getRoot() { return &m_rootLog; }
		Log *getLog(const char *pName)
		{
			LogMap::iterator iter = m_logs.find(pName);
			if (iter != m_logs.end())
				return iter->second;
			else
				return new Log(pName);
		}

		//////////////////////////////////////////////////////////////////////////

		static LogManager &getInstance()	{ static LogManager root; return root; }
		
	protected:
		Log		m_rootLog;
		LogMap	m_logs;
	};


#define LOG_MANAGER					logging::LogManager::getInstance()
#define LOG_GET_ROOT				LOG_MANAGER.getRoot()
#define LOG_GET(NAME)				LOG_MANAGER.getLog(NAME)

#define LOG_CREATE_MSG(EXPR)		std::stringstream _MSG; _MSG << EXPR << std::endl
#define LOG_CHECK(LOG, LEVEL)		(LOG->getLevelRecursive() <= LEVEL)
#define LOG_LOCATION				logging::Location(__FILE__, __LINE__, __FUNCTION__)
#define LOG_MSG(LOG, LEVEL, EXPR)	if (LOG_CHECK(LOG, LEVEL)) { LOG_CREATE_MSG(EXPR); LOG->write(logging::Message(*LOG, LEVEL, LOG_LOCATION, _MSG.str())); } (void)(0)

#define LOG_FATAL(LOG, EXPR)		LOG_MSG(LOG, LEVEL_FATAL, EXPR)
#define LOG_ERROR(LOG, EXPR)		LOG_MSG(LOG, LEVEL_ERROR, EXPR)
#define LOG_WARN(LOG, EXPR)			LOG_MSG(LOG, LEVEL_WARN, EXPR)
#define LOG_INFO(LOG, EXPR)			LOG_MSG(LOG, LEVEL_INFO, EXPR)
#define LOG_DEBUG(LOG, EXPR)		LOG_MSG(LOG, LEVEL_DEBUG, EXPR)
#define LOG_TRACE(LOG, EXPR)		LOG_MSG(LOG, LEVEL_TRACE, EXPR)

//! Append a formatted string using boost::format
#define LOG_FMT(FMT, EXPR)			boost::format(FMT) % EXPR

} //namespace logging

#endif //__UTIL_LOGGING_ROOT_H__
