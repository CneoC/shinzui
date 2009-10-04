#pragma once

#ifndef __WORLD_COMPONENT_H__
#define __WORLD_COMPONENT_H__

#include "core/Process.h"
#include "world/Entity.h"
#include "world/EntityData.h"

namespace world
{
	class Component
		: public core::Process
	{
	public:
		typedef std::map<EntityId, EntityData *>	EntityList;

		virtual void update(EntityId id, EntityData *pData, double delta) = 0;

		virtual core::Process *run(u32 job, double delta)
		{
			EntityList::iterator entity;
			for (entity = m_entities.begin(); entity != m_entities.end(); ++entity)
			{
				update(entity->first, entity->second);
			}
		}

	protected:
		EntityList	m_entities;
	};
}


#endif //__WORLD_COMPONENT_H__
