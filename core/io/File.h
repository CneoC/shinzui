#pragma once

#ifndef __CORE_FILE_H__
#define __CORE_FILE_H__

namespace core
{
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

		File()
			: m_open(false)
		{
		}

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

#endif //__CORE_FILE_H__
