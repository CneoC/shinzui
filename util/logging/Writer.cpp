#include "Writer.h"

#include "Formatter.h"
#include "Message.h"

#include <iostream>

using namespace logging;

boost::mutex ConsoleWriter::ms_consoleMutex;

void ConsoleWriter::write(Message &message)
{
	boost::lock_guard<boost::mutex> lock(ms_consoleMutex);
	std::cout << (m_pFormatter? m_pFormatter->format(message): message.getMessage());
}