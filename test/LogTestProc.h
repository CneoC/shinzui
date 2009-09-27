#pragma once

#ifndef __TEST_LOGTESTPROC_H__
#define __TEST_LOGTESTPROC_H__

#include "core/Process.h"

#include "util/logging/LogManager.h"

class LogTestProc
	: public Process
{
public:
	LogTestProc(Core *pCore, int id = 0, int targetThread = Core::THREAD_ID_NORMAL_MASK)
		: Process(pCore, id, targetThread)
	{
	}

	virtual Process *run(double delta)
	{
		logging::Log *log = LOG_GET_ROOT;
		//LOG_WARN(log, "TEST: " << getId() << " from " << boost::this_thread::get_id());
		return this;
	}
};

#endif //__TEST_LOGTESTPROC_H__
