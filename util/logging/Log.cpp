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
// Log.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "Log.h"

#include "LogManager.h"
#include "Writer.h"
#include "Message.h"
#include "Level.h"

using namespace logging;

Log::Log()
	: m_pName(NULL)
	, m_pLevel(&LEVEL_ALL)
	, m_pParent(NULL)
	, m_recurseWrite(false)
{}

Log::Log(const char *pName)
	: m_pName(pName)
	, m_pLevel(NULL)
	, m_pParent(NULL)
	, m_recurseWrite(true)
{
	LOG_MANAGER.addLog(*this);
}

Log::Log(const char *pName, const Level &level)
	: m_pName(pName)
	, m_pLevel(&level)
	, m_pParent(NULL)
	, m_recurseWrite(true)
{
	LOG_MANAGER.addLog(*this);
}

Log::~Log()
{
	if (m_pName)
		LOG_MANAGER.removeLog(*this);
	clearWriters();
}

void Log::write(Message &message)
{
	if (m_recurseWrite && m_pParent)	m_pParent->write(message);
	WriterList::iterator writer;
	for (writer = m_writers.begin(); writer != m_writers.end(); ++writer)
	{
		Writer *pWriter = *writer;
		if (!pWriter->getLevel() || *pWriter->getLevel() <= message.getLevel())
			pWriter->write(message);
	}
}

void Log::addWriter(Writer *pWriter)
{
	m_writers.push_back(pWriter);
	pWriter->open();
}

void Log::clearWriters()
{
	WriterList::iterator writer;
	for (writer = m_writers.begin(); writer != m_writers.end(); ++writer)
	{
		(*writer)->close();
	}
	m_writers.clear();
}
