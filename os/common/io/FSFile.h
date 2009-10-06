#pragma once

#ifndef __OS_FSFILE_H__
#define __OS_FSFILE_H__

#include "File.h"

#include <boost/filesystem/path.hpp>

namespace os
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

#endif //__OS_FSFILE_H__
