#pragma once

#ifndef __LOGGING_WRITER_H__
#define __LOGGING_WRITER_H__

#include "core/types.h"

#include <boost/thread.hpp>

namespace logging
{
	class Formatter;
	class Message;

	/**
	 * Log writer base class, responsible for writing actual messages to a target.
	 */
	class Writer
	{
	public:
		Writer()
			: m_pFormatter(NULL)
		{}

		virtual void open() = 0;
		virtual void close() = 0;

		virtual void write(Message &message) = 0;

		virtual const char *getName() = 0;

		void setFormatter(Formatter *pFormatter)	{ m_pFormatter = pFormatter; }
		Formatter *getFormatter() const				{ return m_pFormatter; }

	protected:
		Formatter *m_pFormatter;
	};

	/**
	 * Writer that outputs messages to the command prompt
	 */
	class ConsoleWriter
		: public Writer
	{
	public:
		virtual void open() {}
		virtual void close() {}

		virtual void write(Message &message);
		virtual const char *getName() { return "ConsoleWriter"; }

	protected:
		static boost::mutex ms_consoleMutex;
	};

} //namespace logging

#endif //__LOGGING_WRITER_H__
