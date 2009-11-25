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
// PythonScriptEngine.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#ifndef __SCRIPT_PYTHONSCRIPTENGINE_H__
#define __SCRIPT_PYTHONSCRIPTENGINE_H__

#include "ScriptEngine.h"

#include "ScriptObject.h"

#include <boost/python/object.hpp>

namespace bp = boost::python;

namespace script
{
	class Script;

	/**
	* Script engine class which manages and runs a group of scripts.
	*/
	class PythonScriptEngine
		: public ScriptEngine
	{
	public:
		/**
		 * Constructs a script engine.
		 * @param pCore	Core class.
		 * @param id		Process identifier for lookups.
		 */
		PythonScriptEngine(core::Core *pCore, u32 id = 0);

		/**
		 * Executes a simple script command in this script engine.
		 * @param cmd	The command to execute.
		 * @param flags	Flags for the command to execute.
		 */
		virtual PythonScriptObject exec(const std::string &cmd, u32 flags = 0);

		/**
		 * Evaluates an expression in this script engine.
		 * @param cmd	The expression to evaluate.
		 * @param flags	Flags for the expression to evaluate.
		 */
		virtual PythonScriptObject eval(const std::string &cmd, u32 flags = 0);

	protected:
		bp::object	m_mainModule;
		bp::object	m_mainNamespace;
	};
}

#endif /* __SCRIPT_PYTHONSCRIPTENGINE_H__ */
