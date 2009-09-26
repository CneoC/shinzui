#pragma once

#ifndef __UTIL_H__
#define __UTIL_H__

#include "core/types.h"

namespace Util
{
	/**
	 * Fast string hashing function.
	 * This is a slightly modified version from Paul Hsieh (http://www.azillionmonkeys.com/qed/hash.html)
	 */
	u32 hashString(const char *data, u32 len);
};


#endif //__UTIL_H__