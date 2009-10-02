#pragma once

#ifndef __CONSOLE_CONSOLE_H__
#define __CONSOLE_CONSOLE_H__

#include "render/Renderer.h"

namespace console
{
	class Console
		: public render::Renderer
	{
	public:
		Console(Core *pCore);
		~Console();

		virtual void render(double delta);

	protected:
	};
}

#endif //__CONSOLE_CONSOLE_H__
