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
// Script.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#ifndef __SCRIPT_SCRIPTOBJECT_H__
#define __SCRIPT_SCRIPTOBJECT_H__

#include <boost/python/object.hpp>
#include <boost/python/extract.hpp>

namespace bp = boost::python;

namespace script
{
	/**
	 * Really simple wrapper around boost::python::object to allow easy switch between script engines.
	 */
	class PythonScriptObject
	{
	public:
		PythonScriptObject(const bp::object &object)
			: m_object(object)
		{}

		/**
		 * Conversion operator to get C type from Python object.
		 * @return Converted type
		 */
		template < class T >
		operator T () const		{ return bp::extract< T >(m_object); }

	protected:
		bp::object m_object;
	};
}

#endif /* __SCRIPT_SCRIPT_H__ */
