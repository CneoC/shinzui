#pragma once

#ifndef __WORLD_NAMECOMPONENT_H__
#define __WORLD_NAMECOMPONENT_H__

#include "Component.h"

namespace world
{
	struct NameData
		: public ComponentData
	{
		std::string name;
	};

	class NameComponent
		: public Component
	{
	public:
		NameComponent(core::Core *pCore, ComponentManager *pManager)
			: Component(pCore, pManager)
		{}

		//////////////////////////////////////////////////////////////////////////

		virtual ComponentData *createData(const Entity &entity)
		{
			return entity.getData()->create<NameData>("Name");
		}

		virtual void start(double delta) {}
		virtual bool step(double delta) {}

	protected:
	};
}


#endif //__WORLD_NAMECOMPONENT_H__
