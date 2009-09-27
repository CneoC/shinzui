#pragma once

#ifndef __LOG_H__
#define __LOG_H__

#include <iostream>
#include <sstream>
#include <map>
#include <boost/thread.hpp>

#include "core/types.h"

namespace logging
{

/**
 * Class that defines a single log level type.
 */
class Level
{
public:
	/**
	 * Constructs a log level.
	 */
	Level(u32 level, const char *pName, const char *pNeatName)
		: m_level(level)
		, m_pName(pName)
		, m_pNeatName(pNeatName)
	{
	}

	u32 getLevel() const			{ return m_level; }
	const char *getName() const		{ return m_pName; }
	const char *getNeatName() const	{ return m_pNeatName; }

	bool operator < (const Level &other) const	{ return m_level < other.getLevel(); }
	bool operator > (const Level &other) const	{ return m_level > other.getLevel(); }
	bool operator == (const Level &other) const	{ return m_level == other.getLevel(); }

protected:
	u32			m_level;
	const char *m_pName;
	const char *m_pNeatName;
};

/**
 * Class that allows access to al log levels.
 */
class Levels
{
public:
#define DEFINE_LEVEL(LEVEL, NAME, NEAT_NAME) static Level &get##NEAT_NAME()	{ static Level level(LEVEL, #NAME, #NEAT_NAME); return level; }
	DEFINE_LEVEL(0x0000,	LOG_ALL,	All)
	DEFINE_LEVEL(0x1000,	LOG_FATAL,	Fatal)
	DEFINE_LEVEL(0x2000,	LOG_ERROR,	Error)
	DEFINE_LEVEL(0x3000,	LOG_WARN,	Warn)
	DEFINE_LEVEL(0x4000,	LOG_INFO,	Info)
	DEFINE_LEVEL(0x5000,	LOG_DEBUG,	Debug)
	DEFINE_LEVEL(0x6000,	LOG_TRACE,	Trace)
#undef DEFINE_LEVEL
};

// Cleaner way to access levels
#define LEVEL_ALL	logging::Levels::getAll()
#define LEVEL_FATAL	logging::Levels::getFatal()
#define LEVEL_ERROR	logging::Levels::getError()
#define LEVEL_WARN	logging::Levels::getWarn()
#define LEVEL_INFO	logging::Levels::getInfo()
#define LEVEL_DEBUG	logging::Levels::getDebug()
#define LEVEL_TRACE	logging::Levels::getTrace()


class Log;
class Root;

/**
 * Class that represents a location in the code where a message was sent from.
 */
class Location
{
public:
	Location(const char *pFile, u32 line, const char *pFunction)
		: m_pFile(pFile)
		, m_line(line)
		, m_pFunction(pFunction)
	{}

	const char *getFile() const		{ return m_pFile; }
	u32			getLine() const		{ return m_line; }
	const char *getFunction() const	{ return m_pFunction; }

protected:
	const char *m_pFile;
	u32			m_line;
	const char *m_pFunction;
};

/**
 * Class that contains information about a single log message.
 */
class LogMessage
{
public:
	LogMessage(Log &log, const Level &level, const Location &location, const std::string &message)
		: m_log(log)
		, m_level(level)
		, m_location(location)
		, m_message(message)
	{}

	Log &getLog() const						{ return m_log; }
	const Level &getLevel() const			{ return m_level; }
	const Location &getLocation() const		{ return m_location; }
	const std::string &getMessage() const	{ return m_message; }

protected:
	Log &				m_log;
	const Level &		m_level;
	const Location &	m_location;
	const std::string &	m_message;
};

/**
* Log formatter base class, formats messages written by the writer.
*/
class Formatter
{
public:
	virtual std::string format(LogMessage &message) = 0;
};

class SimpleTextFormatter
	: public Formatter
{
public:

};

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

	virtual void write(LogMessage &message) = 0;

	virtual const char *getName() = 0;

	void setFormatter(Formatter *pFormatter)	{ m_pFormatter = pFormatter; }
	Formatter *getFormatter() const				{ return m_pFormatter; }

protected:
	Formatter *m_pFormatter;
};

class ConsoleWriter
	: public Writer
{
public:
	virtual void open() {}
	virtual void close() {}

	virtual void write(LogMessage &message)
	{
		boost::lock_guard<boost::mutex> lock(ms_consoleMutex);
		std::cout << (m_pFormatter? m_pFormatter->format(message): message.getMessage());
	}

	virtual const char *getName() { return "ConsoleWriter"; }

protected:
	static boost::mutex ms_consoleMutex;
};

class Log
{
protected:
	friend class Root;

	/**
	 * Constructor used for root log.
	 */
	Log();

public:
	Log(const char *pName);
	Log(const char *pName, const Level &level);
	~Log();

	const char *getName() const				{ return m_pName; }
	
	const Level &getLevelRecursive() const	{ return m_pLevel? *m_pLevel: m_pParent->getLevelRecursive(); }

	const Level *getLevel() const			{ return m_pLevel; }
	void setLevel(const Level &level)		{ m_pLevel = &level; }

	Log *getParent() const					{ return m_pParent; }

	Writer *getWriter() const				{ return m_pWriter; }
	void setWriter(Writer *pWriter) 		{ m_pWriter = pWriter; }

	bool isRecurseWrite() const				{ return m_recurseWrite; }
	void setRecurseWrite(bool recurse)		{ m_recurseWrite = recurse; }

	void write(LogMessage &message)
	{
		if (m_pWriter)						m_pWriter->write(message);
		if (m_recurseWrite && m_pParent)	m_pParent->write(message);
	}

protected:
	const char *	m_pName;
	const Level *	m_pLevel;

	bool			m_recurseWrite;
	Writer *		m_pWriter;

	Log *			m_pParent;
};


class Root
{
public:
	typedef std::map<const char *, Log *> LogMap;

protected:
	Root() {}

public:
	void addLog(Log &log)
	{
		m_logs[log.getName()] = &log;
	}

	void removeLog(Log &log)
	{
		LogMap::iterator iter = m_logs.find(log.getName());
		if (iter != m_logs.end())
			m_logs.erase(iter);
	}

	static Root &getInstance()	{ static Root root; return root; }

	static Log &getRoot() {	static Log log; return log; }
	static Log &getLog(const char *pName)
	{
		LogMap::iterator iter = getInstance().m_logs.find(pName);
		if (iter != getInstance().m_logs.end())
			return *iter->second;
		else
			return *(new Log(pName));
	}

protected:
	LogMap m_logs;
};


#define LOG_CREATE_MSG(EXPR)		std::stringstream _MSG; _MSG << EXPR << std::endl
#define LOG_CHECK(LOG, LEVEL)		(LOG.getLevelRecursive() < LEVEL)
#define LOG_LOCATION				logging::Location(__FILE__, __LINE__, __FUNCTION__)
#define LOG_MSG(LOG, LEVEL, EXPR)	if (LOG_CHECK(LOG, LEVEL)) { LOG_CREATE_MSG(EXPR); LOG.write(logging::LogMessage(LOG, LEVEL, LOG_LOCATION, _MSG.str())); } (void)(0)

#define LOG_FATAL(LOG, EXPR)	LOG_MSG(LOG, LEVEL_FATAL, EXPR)
#define LOG_ERROR(LOG, EXPR)	LOG_MSG(LOG, LEVEL_ERROR, EXPR)
#define LOG_WARN(LOG, EXPR)		LOG_MSG(LOG, LEVEL_WARN, EXPR)
#define LOG_INFO(LOG, EXPR)		LOG_MSG(LOG, LEVEL_INFO, EXPR)
#define LOG_DEBUG(LOG, EXPR)	LOG_MSG(LOG, LEVEL_DEBUG, EXPR)
#define LOG_TRACE(LOG, EXPR)	LOG_MSG(LOG, LEVEL_TRACE, EXPR)


/*
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdarg>

#include <boost/date_time.hpp>
#include <boost/thread.hpp>

#pragma warning(disable: 4996)

enum LogLevel
{
	LOG_NONE,

	LOG_ERROR,
	LOG_WARNING, 
	LOG_INFO, 
	LOG_DEBUG, 
	LOG_DEBUG1, 
	LOG_DEBUG2, 
	LOG_DEBUG3, 
	LOG_DEBUG4, 
	LOG_DEBUG5
};

class LogOutput
{

};

class LogFormat
{

};

class Log
{
public:
	Log(LogOutput *pOutput, LogFormat *pFormat)
		: m_level(LOG_NONE)
		, m_pOutput(pOutput)
		, m_pFormat(pFormat)
	{
	}

	~Log()
	{
		if (m_level != LOG_NONE) end();
	}

	//////////////////////////////////////////////////////////////////////////

	std::ostream& start(LogLevel level, const char *pFile, int line, const char *pFunction)
	{
		assert(m_level == LOG_NONE);

		m_level = level;

		m_pOutput->start();
		m_pFormat->start(level, pFile, line, pFunction);
		return m_stream;
	}

	void end()
	{
		m_stream	<< m_pFormat->end()
					<< m_pOutput->end();
		m_level = LOG_NONE;
	}

	std::ostream& get()
	{
		return m_stream;
	}

public:
	void setLevel(LogLevel level)		{ m_level = level; }
	LogLevel getLevel() const			{ return m_level; }

	void setOutput(LogOutput *pOutput)	{ m_pOutput = pOutput; }
	LogOutput *setOutput() const		{ return m_pOutput; }

	void setFormat(LogFormat *pFormat)	{ m_pFormat = pFormat; }
	LogFormat *getFormat(pFormat) const	{ return m_pFormat; }

protected:
	LogOutput *m_pOutput;
	LogFormat *m_pFormat;

	LogLevel			m_level;
	std::ostringstream	m_stream;
};

//////////////////////////////////////////////////////////////////////////

class LogOutputUtil
{
public:
	struct printfmt
	{
		printfmt(const char *fmt, ...)
		{
			assert(fmt);

			// Parse and print variable argument list.
			va_list	ap;
			va_start(ap, fmt);
			size_t length = vsprintf(m_text, fmt, ap);
			va_end(ap);
		}

		friend std::ostringstream & operator << (std::ostringstream &os, const printfmt &f)
		{
			os << f.m_text;
			return os;
		}

		char m_text[1024];
	};
};

class LogFormatText
{
public:
	struct time
	{
		friend std::ostringstream & operator << (std::ostringstream &os, const time &t)
		{
			boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
			boost::posix_time::time_facet *facet = new boost::posix_time::time_facet("%H:%M:%S.%f");

			std::stringstream ss;
			ss.imbue(std::locale(std::locale::classic(), facet));
			ss << '[' << time << "] ";

			os << ss.str();
			return os;
		}
	};

	struct thread
	{
		friend std::ostringstream & operator << (std::ostringstream &os, thread &)
		{
			os << "TR=" << boost::this_thread::get_id() << ' ';
			return os;
		}
	};

	struct level
	{
		level(LogLevel l)
			: m_level(l)
		{
		}

		friend std::ostringstream & operator << (std::ostringstream &os, level &l)
		{
			os << "\t[" << getLevelString(l.m_level) << "] ";
			os << std::string(l.m_level > LOG_DEBUG ? l.m_level - LOG_DEBUG : 0, '\t');
			return os;
		}

		static const char *getLevelString(LogLevel level)
		{
			static const char *const buffer[] = {"ERROR  ", "WARNING", "INFO   ", "DEBUG  ", "DEBUG01", "DEBUG02", "DEBUG03", "DEBUG04", "DEBUG05"};
			return buffer[level];
		}

		LogLevel m_level;
	};

	struct file
	{
		file(const char *pFile, int line, const char *pFunction)
			: m_pFile(pFile)
			, m_line(line)
			, m_pFunction(pFunction)
		{
		}

		friend std::ostringstream & operator << (std::ostringstream &os, file &f)
		{
			os << f.m_pFunction << "->" << f.m_pFile << '(' << f.m_line << ')' << std::endl;
			return os;
		}

		const char *m_pFile;
		int m_line;
		const char *m_pFunction;
	};

	struct start
	{
		start(LogLevel level, const char *pFile, int line, const char *pFunction)
			: m_level(level)
			, m_pFile(pFile)
			, m_line(line)
			, m_pFunction(pFunction)
		{
		}

		friend std::ostringstream & operator << (std::ostringstream &os, start &s)
		{
			os  << time()
				<< thread()
				<< file(s.m_pFile, s.m_line, s.m_pFunction)
				<< level(s.m_level);
			return os;
		}

		LogLevel m_level;
		const char *m_pFile;
		int m_line;
		const char *m_pFunction;
	};

	struct end
	{
		friend std::ostringstream & operator << (std::ostringstream &os, end &)
		{
			os << std::endl;
			return os;
		}
	};
};

class LogOutputFile
{
public:
	static FILE*& getStream()
	{
		static FILE *pStream = stderr;
		return pStream;
	}

	struct start
	{
		start(LogLevel level)
			: m_level(level)
		{
		}

		friend std::ostringstream & operator << (std::ostringstream &os, start &s)
		{
			return os;
		}

		LogLevel m_level;
	};

	struct end
	{
		friend std::ostringstream & operator << (std::ostringstream &os, end &)
		{
			FILE *pStream = getStream();
			if (pStream)
			{
				std::string str = os.str();
				fwrite(str.c_str(), str.size(), 1, pStream);
				fflush(pStream);
			}
			return os;
		}
	};
};

// Log utilities.
#define LOG_CHECK(LOG) \
	(LOG.getLevel() <= LOG.getReportLevel())

#define LOG_PRINTF(FMT, ...)	LogOutputUtil::printfmt(FMT, __VA_ARGS__)

// Temporary variable based log defines.
#define LOG_LINE(TYPE, LEVEL)	if (LOG_CHECK(TYPE, LEVEL)) TYPE().start(LEVEL, __FILE__, __LINE__, __FUNCTION__)

// Variable based log defines.
#define LOGV_START(LOG, LEVEL)	if (LOG_CHECK(LOG)) LOG.start(LEVEL, __FILE__, __LINE__, __FUNCTION__)
#define LOGV_GET(LOG)			if (LOG_CHECK(LOG)) LOG.get()
#define LOGV_END(LOG)			if (LOG_CHECK(LOG)) LOG.end()
*/

} //namespace log

#endif //__LOG_H__
