#include "WindowBase.h"

WindowBase::WindowBase(Core *pCore)
	: Process(pCore, 0, Core::THREAD_ID_CORE_BIT)
	, m_size(800, 600)
	, m_fullscreen(false)
	, m_bpp(0)
{
	setFrameDelay(0.1);
}

WindowBase::~WindowBase()
{
}
