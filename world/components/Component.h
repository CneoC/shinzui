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
