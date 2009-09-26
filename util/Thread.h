#pragma once

#ifndef __THREAD_H__
#define __THREAD_H__

#include <boost/thread.hpp>

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

public:
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

#endif //__THREAD_H__
