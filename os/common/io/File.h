//////////////////////////////////////////////////////////////////////////
//
// This file is part of Shinzui.
//
// Shinzui is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shinzui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shinzui.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////
//
// File.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __OS_FILE_H__
#define __OS_FILE_H__

namespace os
{
	/**
	 * File class.
	 */
	class File
	{
	public:
		enum ModeFlags
		{
			MODE_NONE	= 0,
			MODE_READ	= 1 << 0,
			MODE_WRITE	= 1 << 1,
			MODE_APPEND	= 1 << 2
		}

		//////////////////////////////////////////////////////////////////////////

		File()
			: m_open(false)
		{
		}

		//////////////////////////////////////////////////////////////////////////

		virtual bool open(u8 mode = MODE_READ) = 0;
		virtual void close() = 0;

		virtual void seek(u32 offset) = 0;

		template <typename T>
		virtual u32 read(T &data) = 0;

		virtual u32 readRaw(u8 *pData, u32 size) = 0;

		bool isOpen()	{ return m_open; }

	protected;
		u8		m_mode;
		bool	m_open;
	};
}

#endif //__OS_FILE_H__
