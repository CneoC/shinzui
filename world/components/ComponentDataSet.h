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
// ComponentDataSet.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __WORLD_COMPONENTDATASET_H__
#define __WORLD_COMPONENTDATASET_H__

#include "core/types.h"
#include "util/Util.h"

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

namespace world
{
	struct ComponentData
	{
		u32			_hash;
		std::string	_name;
	};

	class ComponentDataSet
	{
	public:
		typedef std::map<u32, ComponentData *> DataList;

		/**
		 * Creates a new data object with a specific name and type.
		 * @return The created (or previously existing) component data.
		 */
		template <class T>
		T *create(const std::string &name)
		{
			u32 hash = util::hashString(name.c_str(), name.length());
			
			// Check for existing data
			T *pData = get<T>(hash);
			if (pData)
				return pData;

			// Create new data
			pData = new T;
			pData->_hash	= hash;
			pData->_name	= name;
			m_data[pData->_hash] = pData;
			return pData;
		}

		/**
		 * Gets component data by name as a specific type.
		 */
		ComponentData *get(const std::string &name) const
		{
			return get(util::hashString(name.c_str(), name.length()));
		}
		template <class T> T *get(const std::string &name) const { return static_cast<T *>(get(name)); }

		/**
		 * Gets component data by hash as a specific type.
		 */
		ComponentData *get(u32 hash) const
		{
			DataList::const_iterator found = m_data.find(hash);
			if (found != m_data.end())
				return found->second;
			return NULL;
		}
		template <class T> T *get(u32 hash) const { return static_cast<T *>(get(hash)); }

	protected:
		DataList	m_data;
	};

	typedef boost::shared_ptr<ComponentDataSet>	ComponentDataSetRef;
}


#endif //__WORLD_COMPONENTDATASET_H__
