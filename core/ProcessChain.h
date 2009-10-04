#pragma once

#ifndef __CORE_PROCESSCHAIN_H__
#define __CORE_PROCESSCHAIN_H__

#include "Process.h"

namespace core
{
	class ProcessChain
		: public Process
	{
	public:
		/**
		* Constructs a chained process.
		* @param pCore			core class.
		* @param id				process identifier for lookups.
		* @param threadMask	target thread id to run this process on (THREAD_ID_NONE for any thread)
		*/
		ProcessChain(core::Core *pCore, int id = 0, int threadMask = core::Core::THREAD_ID_NORMAL_MASK)
			: Process(pCore, id, threadMask)
			, m_pNext(NULL)
		{
		}

		/**
		* Runs the process.
		* @return if the process is added to the run queue after execution.
		*/
		virtual Process *run(u32 job, double delta)
		{
			return m_pNext ? m_pNext : this;
		}

		//! Utility function to chain processes cleaner.
		ProcessChain *link(ProcessChain *pProcess)	{ setNext(pProcess); return pProcess; }
		//! Utility function to chain processes cleaner.
		void link(Process *pProcess)				{ setNext(pProcess); }

		//! Sets the next process in the chain.
		void setNext(Process *pNext)	{ m_pNext = pNext; m_pNext->setThreadMask(getThreadMask()); }
		//! Gets the next process in the chain.
		Process *getNext()const			{ return m_pNext; }

	protected:
		Process	*m_pNext;
	};
}

#endif //__CORE_PROCESSCHAIN_H__
