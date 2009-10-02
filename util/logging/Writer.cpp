#include "Writer.h"

#include "Formatter.h"
#include "Message.h"

#include <iostream>

using namespace logging;

boost::mutex ConsoleWriter::ms_mutex;
boost::mutex FileWriter::ms_mutex;
//////////////////////////////////////////////////////////////////////////

void ConsoleWriter::write(Message &message)
{
	boost::lock_guard<boost::mutex> lock(ms_mutex);
	std::cout << (m_pFormatter? m_pFormatter->format(message): message.getMessage());
}

//////////////////////////////////////////////////////////////////////////

void FileWriter::open()
{
	if (m_openCount == 0)
	{
		m_file.open(m_path.native_file_string().c_str(), std::ios::out | std::ios::app);

		if (m_pFormatter)
			m_file << m_pFormatter->getHeader();
	}
	m_openCount++;
}

void FileWriter::close()
{
	m_openCount--;
	if (m_openCount == 0)
	{
		if (m_pFormatter)
			m_file << m_pFormatter->getFooter();

		m_file.close();
	}
}

void FileWriter::write(Message &message)
{
	boost::lock_guard<boost::mutex> lock(ms_mutex);
	m_file << (m_pFormatter? m_pFormatter->format(message): message.getMessage());
}