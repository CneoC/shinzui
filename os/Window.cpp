#include "Window.h"

Window::Window(Core *pCore)
	: ChainProcess(pCore, 0, Core::THREAD_ID_MAIN)
	, m_size(800, 600)
	, m_fullscreen(false)
	, m_bpp(0)
{
}

Window::~Window()
{
}
