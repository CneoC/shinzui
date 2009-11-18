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
// Writer.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "Writer.h"

#include "Formatter.h"
#include "Message.h"

#include <iostream>

using namespace logging;

boost::mutex ConsoleWriter::ms_mutex;
boost::mutex FileWriter::ms_mutex;
//////////////////////////////////////////////////////////////////////////

void ConsoleWriter::write(Message &message)
{
	boost::lock_guard<boost::mutex> lock(ms_mutex);
	std::cout << (m_pFormatter? m_pFormatter->format(message): message.getMessage());
}

//////////////////////////////////////////////////////////////////////////

void FileWriter::open()
{
	if (m_openCount == 0)
	{
		m_file.open(m_path.native_file_string().c_str(), std::ios::out | std::ios::app);

		if (m_pFormatter)
			m_file << m_pFormatter->getHeader();
	}
	m_openCount++;
}

void FileWriter::close()
{
	m_openCount--;
	if (m_openCount == 0)
	{
		if (m_pFormatter)
			m_file << m_pFormatter->getFooter();

		m_file.close();
	}
}

void FileWriter::write(Message &message)
{
	boost::lock_guard<boost::mutex> lock(ms_mutex);
	m_file << (m_pFormatter? m_pFormatter->format(message): message.getMessage());
	m_file.flush();
}