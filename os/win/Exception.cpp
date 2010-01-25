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
// Exception.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#ifdef _WIN32 

#include "Exception.h"

using namespace os;

Exception::Exception(const char *pReason,
					 const char *pFile,
					 const u32 line
					 )
	: std::exception(pReason)
	, m_pFile(pFile)
	, m_line(line)
{
	// TODO: Implement stack trace support
}

Exception::Exception(const Exception &copy)
	: std::exception(copy)
	, m_pFile(copy.m_pFile)
	, m_line(copy.m_line)
	, m_stack(copy.m_stack)
{
}

Exception & Exception::operator = (const Exception &e)
{
	if (this != &e)
	{
		std::exception::operator = (e);

		m_pFile = e.m_pFile;
		m_line = e.m_line;
		m_stack = e.m_stack;
	}
	return *this;
}

#endif // _WIN32