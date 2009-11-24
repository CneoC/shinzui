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

#ifdef _linux_

#include "Exception.h"

#include <execinfo.h>

using namespace os;

Exception::Exception(const char *pReason,
					 const char *pFile,
					 const u32 line
					 )
	: std::exception(pReason)
	, m_pFile(pFile)
	, m_line(line)
{
	// Acquire the stack trace...
	void * array[25];
	size_t entries = backtrace( array, sizeof( array ) / sizeof( void* ) );
	char ** symbols = backtrace_symbols( array, entries );
	if ( symbols == 0 ) {
		// Probable out of memory condition; we'll skip
		// over the full stack.
		entries = 0;
	}

	// Throw it into our Vector.  We are skipping the
	// top-most element, since this will be this
	// constructor, which is not particularly interesting.
	// We are also skipping the next line, because the
	// __FILE__ and __LINE__ have that location already.
	stack_.reserve( entries > 2 ? entries - 2 : 0 );
	for ( size_t i = 2; i < entries; i++ ) {
		stack_.push_back( symbols[i] );
	}

	// Free up the allocated memory.
	free( symbols );
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

#endif /* _linux_ */