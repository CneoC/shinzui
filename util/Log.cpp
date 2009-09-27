#include "Log.h"

using namespace logging;

//////////////////////////////////////////////////////////////////////////

boost::mutex ConsoleWriter::ms_consoleMutex;

//////////////////////////////////////////////////////////////////////////

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
	Root::getInstance().addLog(*this);
}

Log::Log(const char *pName, const Level &level)
	: m_pName(pName)
	, m_pLevel(&level)
	, m_pParent(NULL)
	, m_recurseWrite(true)
	, m_pWriter(NULL)
{
	Root::getInstance().addLog(*this);
}

Log::~Log()
{
	Root::getInstance().removeLog(*this);
}
