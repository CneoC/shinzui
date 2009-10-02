#pragma once

#ifndef __LOGGING_FORMATTER_H__
#define __LOGGING_FORMATTER_H__

#include "FormatUtil.h"

namespace logging
{
	class Message;

	/**
	 * Log formatter base class, formats messages written by the writer.
	 */
	class Formatter
	{
	public:
		virtual std::string getHeader() = 0;
		virtual std::string getFooter() = 0;
		virtual std::string format(Message &message) = 0;
	};

	class TextFormatter
		: public Formatter
	{
	public:
		virtual std::string getHeader()
		{
			std::ostringstream out;
			out << std::endl;
			out << "//////////////////////////////////////////////////////////////////////////" << std::endl;
			out << "// ";
			out << FormatUtil::datetime() << std::endl;
			out << "//////////////////////////////////////////////////////////////////////////" << std::endl;
			out << std::endl;
			return out.str();
		}

		virtual std::string getFooter()
		{
			std::ostringstream out;
			out << std::endl;
			out << "//////////////////////////////////////////////////////////////////////////" << std::endl;
			return out.str();
		}

		virtual std::string format(Message &message)
		{
			std::ostringstream out;
			// Extensive logs for ERROR level logs or higher
			if (message.getLevel() >= LEVEL_ERROR)
			{
				out << FormatUtil::level(message)
					<< FormatUtil::logger(message)
					<< FormatUtil::time(message)
					<< FormatUtil::location(message)
					<< std::endl
					<< '\t' << message.getMessage();
			}
			// Simple logs for the rest
			else
			{
				out << FormatUtil::level(message)
					<< FormatUtil::logger(message)
					<< FormatUtil::time(message)
					<< FormatUtil::function(message)
					<< ' ' << message.getMessage();
			}

			return out.str();
		}

	};

}

#endif //__LOGGING_FORMATTER_H__
