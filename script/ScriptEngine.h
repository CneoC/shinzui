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
// ScriptEngine.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#ifndef __SCRIPT_SCRIPTENGINE_H__
#define __SCRIPT_SCRIPTENGINE_H__

#include "core/Process.h"

#include "ScriptObject.h"

namespace script
{
	class Script;

	/**
	 * Script engine class which manages and runs a group of scripts.
	 */
	class ScriptEngine
		: public core::Process
	{
	public:
		enum ExecFlags
		{
			EXEC_QUEUE	= 0x0001,	//!< Queue the command for execution (for multithreading).
		};

		/**
		 * Constructs a script engine.
		 * @param pCore	Core class.
		 * @param id		Process identifier for lookups.
		 */
		ScriptEngine(core::Core *pCore, u32 id = 0)
			: core::Process(pCore, id)
		{
		}

		/**
		 * @see core::Process::onStart
		 */
		virtual void onStart() {}

		// TODO: generalize more of the script based logic?

	protected:
	};
}

#endif /* __SCRIPT_SCRIPTENGINE_H__ */