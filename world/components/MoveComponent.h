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
// MoveComponent.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __WORLD_MOVECOMPONENT_H__
#define __WORLD_MOVECOMPONENT_H__

#include "Component.h"
#include "TransformData.h"

#include "math/Vector3.h"

namespace world
{
	struct MoveData
		: public ComponentData
	{
		math::Vector3f	velocity;
		math::Vector3f	damping;
		math::Vector3f	gravity;
	};

	class MoveComponent
		: public Component
	{
	public:
		MoveComponent(core::Core *pCore, ComponentManager *pManager);

		//////////////////////////////////////////////////////////////////////////

		virtual ComponentData *createData(const Entity &entity);

		virtual void onStart();
		bool step();

	protected:
		boost::mutex			m_mutex;
		EntityList::iterator	m_entity;
		u32						m_transformHash;
	};
}


#endif //__WORLD_MOVECOMPONENT_H__
