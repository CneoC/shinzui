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
// LogTestProc.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __TEST_LOGTESTPROC_H__
#define __TEST_LOGTESTPROC_H__

#include "core/Process.h"

#include "util/logging/LogManager.h"

namespace test
{
	class LogTestProc
		: public core::Process
	{
	public:
		LogTestProc(core::Core *pCore, int id = 0)
			: Process(pCore, id)
		{
		}

		virtual Process *run(u32 job, double delta)
		{
			logging::Log *log = LOG_GET_ROOT;
			//LOG_WARN(log, "TEST: " << getId() << " from " << boost::this_thread::get_id());
			return this;
		}
	};
}

#endif //__TEST_LOGTESTPROC_H__
