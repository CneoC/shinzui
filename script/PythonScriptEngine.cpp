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

#include "PythonScriptEngine.h"

//#include "PythonScriptObject.h"

#include <boost/python.hpp>

namespace bp = boost::python;

using namespace script;

PythonScriptEngine::PythonScriptEngine(core::Core *pCore, u32 id)
	: ScriptEngine(pCore, id)
{
	Py_Initialize();
	PyEval_InitThreads();

	m_mainModule	= bp::import("__main__");
	m_mainNamespace	= m_mainModule.attr("__dict__");
}

PythonScriptObject PythonScriptEngine::exec(const std::string &cmd, u32 flags)
{
	// Lock the thread
	PyGILState_STATE gstate;
	gstate = PyGILState_Ensure();

	bp::object o = bp::exec(cmd.c_str(), m_mainNamespace);

	// Release the thread
	PyGILState_Release(gstate);

	return o;//PythonScriptObject(o);
}

PythonScriptObject PythonScriptEngine::eval(const std::string &cmd, u32 flags)
{
	// Lock the thread
	PyGILState_STATE gstate;
	gstate = PyGILState_Ensure();

	bp::object o = bp::eval(cmd.c_str(), m_mainNamespace);

	// Release the thread
	PyGILState_Release(gstate);

	return o;//PythonScriptObject(o);
}
