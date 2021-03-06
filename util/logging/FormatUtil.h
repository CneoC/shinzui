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
// FormatUtil.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __LOGGING_FORMATUTIL_H__
#define __LOGGING_FORMATUTIL_H__

#include "Message.h"
#include "Level.h"
#include "Log.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>

namespace logging
{
#define DEFINE_UTIL_START(NAME)	struct NAME \
								{ \
									const Message *m; \
									NAME(const Message &message) : m(&message) {} \
									NAME() : m(NULL) {} \
									friend std::ostringstream & operator << (std::ostringstream &out, const NAME &value) \
									{ \
										const Message &m = *value.m;

#define DEFINE_UTIL_END					return out; \
									} \
								};

	class FormatUtil
	{
	public:
		DEFINE_UTIL_START(level)
			out << std::string(m.getLevel().getIndent(), ' ');
			out << "[" << m.getLevel().getName() << "] ";
		DEFINE_UTIL_END

		DEFINE_UTIL_START(time)
			boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
			boost::posix_time::time_facet *facet = new boost::posix_time::time_facet("%H:%M:%S.%f");

			out.imbue(std::locale(std::locale::classic(), facet));
			out << '[' << time << "] ";
		DEFINE_UTIL_END
			
		DEFINE_UTIL_START(datetime)
			boost::posix_time::ptime date = boost::posix_time::microsec_clock::local_time();
			boost::posix_time::time_facet *facet = new boost::posix_time::time_facet("%d-%m-%y %H:%M:%S.%f");

			out.imbue(std::locale(std::locale::classic(), facet));
			out << '[' << date << "] ";
		DEFINE_UTIL_END

		DEFINE_UTIL_START(thread)
			out << "TR=" << boost::this_thread::get_id() << ' ';
		DEFINE_UTIL_END

		DEFINE_UTIL_START(logger)
			if (m.getLog().getName())
				out << m.getLog().getName() << ' ';
			else
				out << ". ";
		DEFINE_UTIL_END

		DEFINE_UTIL_START(function)
			out << m.getLocation().getFunction();
		DEFINE_UTIL_END

		DEFINE_UTIL_START(location)
			out << m.getLocation().getFunction()
				<< "->" << m.getLocation().getFile() 
				<< '(' << m.getLocation().getLine() << ')';
		DEFINE_UTIL_END
	};

#undef DEFINE_UTIL_START
#undef DEFINE_UTIL_END
}


#endif //__LOGGING_FORMATUTIL_H__
