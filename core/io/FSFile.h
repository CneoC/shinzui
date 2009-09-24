#pragma once

#ifndef __FS_FILE_H__
#define __FS_FILE_H__

#include "File.h"

#include <boost/filesystem/path.hpp>

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

#endif //__FS_FILE_H__
