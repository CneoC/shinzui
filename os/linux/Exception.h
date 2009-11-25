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
// Exception.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#ifndef __OS_EXCEPTION_H__
#define __OS_EXCEPTION_H__

#include <exception>
#include <string>
#include <vector>

namespace os
{
	class Exception
		: public std::exception 
	{
	public:
		typedef std::vector<std::string> StackTrace;

		/**
		 * Constructs a new exception.
		 * @param pReason	the reason the exception occurred.
		 * @param pFile		the file the exception occurred in.
		 * @param line		the line the exception occurred on.
		 */
		Exception(
			const char *pReason,
			const char *pFile = NULL,
			u32 line = 0);

		//! Copy constructor.
		Exception(const Exception& ex);
		//! Assignment operator.
		Exception & operator = (const Exception &);

		//////////////////////////////////////////////////////////////////////////
		
		const char *getFilename() const		{ return m_pFile; }
		int getLineNumber() const			{ return m_line; }

		StackTrace getStackTrace() const	{ return m_stack; }

	protected:
		const char *m_pFile;
		const u32 m_line;
		StackTrace m_stack;		// a vector of strings with the stack trace.
	};
}

#define THROW_EXCEPTION(REASON)    throw os::Exception(REASON, __FILE__, __LINE__)

#endif /* __OS_EXCEPTION_H__ */
