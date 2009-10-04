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

		virtual void start(double delta);
		virtual bool step(double delta);

	protected:
		boost::mutex			m_mutex;
		EntityList::iterator	m_entity;
		u32						m_transformHash;
	};
}


#endif //__WORLD_MOVECOMPONENT_H__
