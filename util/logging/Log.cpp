#include "Log.h"

#include "LogManager.h"
#include "Writer.h"
#include "Message.h"
#include "Level.h"

using namespace logging;

Log::Log()
	: m_pName(NULL)
	, m_pLevel(&LEVEL_ALL)
	, m_pParent(NULL)
	, m_recurseWrite(false)
	, m_pWriter(NULL)
{}

Log::Log(const char *pName)
	: m_pName(pName)
	, m_pLevel(NULL)
	, m_pParent(NULL)
	, m_recurseWrite(true)
	, m_pWriter(NULL)
{
	LOG_MANAGER.addLog(*this);
}

Log::Log(const char *pName, const Level &level)
	: m_pName(pName)
	, m_pLevel(&level)
	, m_pParent(NULL)
	, m_recurseWrite(true)
	, m_pWriter(NULL)
{
	LOG_MANAGER.addLog(*this);
}

Log::~Log()
{
	if (m_pName)
		LOG_MANAGER.removeLog(*this);
}

void Log::write(Message &message)
{
	if (m_recurseWrite && m_pParent)	m_pParent->write(message);
	if (m_pWriter)						m_pWriter->write(message);
}
