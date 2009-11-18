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
// Component.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "Component.h"

#include "world/Entity.h"

using namespace world;

//////////////////////////////////////////////////////////////////////////

Component::Component(core::Core *pCore, ComponentManager *pManager)
	: core::Process(pCore)
	, m_pManager(pManager)
{
	m_color = math::Color3f(0, 1, 1);
}

//////////////////////////////////////////////////////////////////////////

void Component::addEntity(const Entity &entity)
{
	ComponentData *pData = createData(entity);
	ComponentEntry &entry = m_entities[entity.getId()];
	entry.data		= createData(entity);
	entry.dataSet	= entity.getData();
}

void Component::removeEntity(const Entity &entity)
{
	EntityList::iterator found = m_entities.find(entity.getId());
	if (found != m_entities.end())
		m_entities.erase(found);
}
