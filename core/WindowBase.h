#pragma once

#ifndef __WINDOW_BASE_H__
#define __WINDOW_BASE_H__

#include "core/ChainProcess.h"

#include "math/Vector2.h"

class WindowBase :
	public ChainProcess
{
public:
	WindowBase(Core *pCore);
	virtual ~WindowBase();

	virtual bool create() = 0;
	virtual bool destroy() = 0;
	virtual bool resize() = 0;

	bool recreate()
	{
		destroy();
		return create();
	}

	const std::string &getTitle() const		{ return m_title; }
	void setTitle(const std::string &title)	{ m_title = title; }

	bool isFullscreen() const				{ return m_fullscreen; }
	void setFullscreen(bool fullsreen)		{ m_fullscreen = fullsreen; }

	const Vector2i &getSize() const			{ return m_size; }
	void setSize(const Vector2i &size)		{ m_size = size; }

	const int getBpp() const				{ return m_bpp; }
	void setBpp(int bpp)					{ m_bpp = bpp; }

protected:
	bool		m_fullscreen;
	std::string	m_title;
	Vector2i	m_size;
	int			m_bpp;
};

#endif //__WINDOW_BASE_H__

