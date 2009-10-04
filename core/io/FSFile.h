#pragma once

#ifndef __CORE_FSFILE_H__
#define __CORE_FSFILE_H__

#include "File.h"

#include <boost/filesystem/path.hpp>

namespace core
{
	class FSFile
		: public File
	{
		using boost::filesystem::Path;

		FSFile()
		{
		}

		FSFile(const Path &path)
		{
		}

	};
}

#endif //__CORE_FSFILE_H__
