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
// Util.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __UTIL_H__
#define __UTIL_H__

#include "core/types.h"

namespace util
{
	/**
	 * Fast string hashing function using 32-bits accumulator.
	 * This is a slightly modified version from Paul Hsieh (http://www.azillionmonkeys.com/qed/hash.html)
	 */
	u32 hashString(const char *data, u32 len, u32 hash = 0);

};


#endif //__UTIL_H__