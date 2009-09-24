#pragma once

#ifndef __LOG_TEST_PROC_H__
#define __LOG_TEST_PROC_H__

#include "core/Process.h"

#include "Log.h"

class LogTestProc
	: public Process
{
public:
	LogTestProc(Core *pCore, int id = 0, int targetThread = Core::THREAD_ID_NORMAL)
		: Process(pCore, id, targetThread)
	{
	}

	virtual Process *run(double delta)
	{
		//LOG(FileLog, LOG_INFO) << "Testing process: " << getId();
		return this;
	}
};

#endif //__LOG_TEST_PROC_H__
