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
// Thread.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __OS_THREAD_H__
#define __OS_THREAD_H__

#include <windows.h>
#include <boost/thread.hpp>

namespace os
{
	/**
	 * Abstract thread class.
	 */
	class Thread
	{
	private:
		/**
		 * Functor to handle the actual running of this thread.
		 */
		class RunThread
		{
		public:
			void operator () (Thread *pThread)
			{
				while (pThread->isRunning())
				{
					if (!pThread->run())
						return;
				}
			}
		};

		//////////////////////////////////////////////////////////////////////////

	public:
		enum Priority
		{
			PRIORITY_REALTIME,
			PRIORITY_HIGH,
			PRIORITY_ABOVE_NORMAL,
			PRIORITY_NORMAL,
			PRIORITY_BELOW_NORMAL,
			PRIORITY_IDLE
		};

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Constructs an empty thread.
		 */
		Thread()
			: m_running(false)
			, m_pThread(NULL)
		{
		}

		/**
		 * Starts the thread.
		 */
		void start()
		{
			m_running = true;
			m_pThread = new boost::thread(RunThread(), this);
		}

		/**
		 * Stops the thread
		 * @param block	waits for the thread to finish
		 */
		void stop(bool block = false)
		{
			m_running = false;
			if (block) 
				m_pThread->join();
		}

		/**
		 * Sets the thread priority.
		 * !!! Only works on win32 right now !!!
		 * @param priority	Priority to set the thread to.
		 */
		bool setPriority(Priority priority)
		{
#ifdef _WIN32
			if (!m_pThread)
				return false;

			HANDLE handle = m_pThread->native_handle();
			switch (priority)
			{
			case PRIORITY_REALTIME:		return SetThreadPriority(handle, THREAD_PRIORITY_TIME_CRITICAL) == TRUE;
			case PRIORITY_HIGH:			return SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST) == TRUE;
			case PRIORITY_ABOVE_NORMAL: return SetThreadPriority(handle, THREAD_PRIORITY_ABOVE_NORMAL) == TRUE;
			case PRIORITY_NORMAL:		return SetThreadPriority(handle, THREAD_PRIORITY_NORMAL) == TRUE;
			case PRIORITY_BELOW_NORMAL:	return SetThreadPriority(handle, THREAD_PRIORITY_BELOW_NORMAL) == TRUE;
			case PRIORITY_IDLE:			return SetThreadPriority(handle, THREAD_PRIORITY_LOWEST) == TRUE;
			}
#endif
			return false;
		}

		/**
		 * Runs the thread
		 * This function is called until it returns false (which stops the thread)
		 *  or until isRunning() returns false.
		 * @return if the thread should continue to run.
		 */
		virtual bool run() = 0;

		//! If the thread is still running.
		bool isRunning() const	{ return m_running; }

	protected:
		bool			m_running;
		boost::thread *	m_pThread;
	};
}

#endif //__UTIL_THREAD_H__
