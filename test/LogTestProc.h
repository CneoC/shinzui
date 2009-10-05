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
