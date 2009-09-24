#pragma once

#ifndef __LOG_H__
#define __LOG_H__
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
			static const char* const buffer[] = {"ERROR  ", "WARNING", "INFO   ", "DEBUG  ", "DEBUG01", "DEBUG02", "DEBUG03", "DEBUG04", "DEBUG05"};
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
		static FILE* pStream = stderr;
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
			FILE* pStream = getStream();
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
#endif //__LOG_H__
