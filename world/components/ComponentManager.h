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
// ComponentManager.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __WORLD_COMPONENTMANAGER_H__
#define __WORLD_COMPONENTMANAGER_H__

#include "core/Process.h"
#include "Component.h"

#include "util/Util.h"

#include <map>
#include <boost/shared_ptr.hpp>

namespace world
{
	class ComponentManager
		: public core::Process
	{
	public:
		typedef std::map<u32, ComponentRef>	ComponentList;

		//////////////////////////////////////////////////////////////////////////

		ComponentManager(core::Core *pCore, u32 id = 0);

		//////////////////////////////////////////////////////////////////////////

		virtual void onStart() {}

		//////////////////////////////////////////////////////////////////////////

		void addComponent(const std::string &name, Component *pComponent)
		{
			addComponent(name, ComponentRef(pComponent));
		}

		void addComponent(const std::string &name, const ComponentRef &component)
		{
			u32 hash = util::hashString(name.c_str(), name.length());
			m_components[hash] = component;
			m_pCore->addProcess(component.get());
		}

		ComponentRef getComponent(const std::string &name)
		{
			u32 hash = util::hashString(name.c_str(), name.length());
			ComponentList::iterator found = m_components.find(hash);
			if (found != m_components.end())
				return found->second;
			return ComponentRef();
		}

	protected:
		ComponentList	m_components;
	};
}

#endif //__WORLD_COMPONENTMANAGER_H__
