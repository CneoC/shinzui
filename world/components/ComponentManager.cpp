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
// ComponentManager.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include "ComponentManager.h"

#include "Component.h"

using namespace world;

ComponentManager::ComponentManager(core::Core *pCore, u32 id)
	: core::Process(pCore, id)
{
	m_color = math::Color3f(1, 1, 0);
}
 
// void ComponentManager::onStart()
// {
// 	ComponentList::iterator component;
// 	for (component = m_components.begin(); component != m_components.end(); ++component)
// 	{
// 		component->second->onStart();
// 	}
// }
