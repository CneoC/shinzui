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
// Component.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __WORLD_COMPONENT_H__
#define __WORLD_COMPONENT_H__

#include "core/Process.h"
#include "world/Entity.h"
#include "world/components/ComponentDataSet.h"

#include <boost/thread.hpp>

namespace world
{
	class ComponentManager;

	class Component
		: public core::Process
	{
	public:
		struct ComponentEntry
		{
			ComponentData *		data;		// quick access slot for current component's data.
			ComponentDataSetRef	dataSet;	// component's complete data set.
		};

		typedef std::map<u32, ComponentEntry>	EntityList;

		//////////////////////////////////////////////////////////////////////////

		Component(core::Core *pCore, ComponentManager *pManager);

		//////////////////////////////////////////////////////////////////////////

		virtual ComponentData *createData(const Entity &entity) { return NULL; }

		void addEntity(const Entity &entity);
		void removeEntity(const Entity &entity);

	protected:
		ComponentManager *	m_pManager;
		EntityList			m_entities;
	};

	typedef boost::shared_ptr<Component> ComponentRef;
}


#endif //__WORLD_COMPONENT_H__
