#include "WindowBase.h"

using namespace os;

//////////////////////////////////////////////////////////////////////////

WindowBase::WindowBase(core::Core *pCore)
	: Process(pCore, 0, core::Core::THREAD_ID_CORE_BIT)
	, m_size(800, 600)
	, m_fullscreen(false)
	, m_bpp(0)
{
	setFrameDelay(0.1);
	m_color = math::Color3f(0, 0, 1);
}

WindowBase::~WindowBase()
{
}
