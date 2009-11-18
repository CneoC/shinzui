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
// Entity.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "Entity.h"

#include "world/components/Component.h"
#include "world/components/ComponentManager.h"

using namespace world;

//////////////////////////////////////////////////////////////////////////

u32				Entity::ms_guid		= ENTITY_NONE + 1;
boost::mutex	Entity::ms_guidMutex;

void Entity::addComponent(const std::string &name)
{
	ComponentRef component = m_pManager->getComponent(name);
	component->addEntity(*this);
}

void Entity::removeComponent(const std::string &name)
{
	ComponentRef component = m_pManager->getComponent(name);
	component->removeEntity(*this);
}