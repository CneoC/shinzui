#pragma once

#ifndef __WORLD_INPUTCOMPONENT_H__
#define __WORLD_INPUTCOMPONENT_H__

#include "core/Process.h"
#include "render/Renderer.h"
#include "world/components/Component.h"

#include "math/Vector3.h"

namespace world
{
	struct InputData
		: public ComponentData
	{

	};

	class InputComponent
		: public Component
	{
	public:
		InputComponent(core::Core *pCore, ComponentManager *pManager)
			: Component(pCore, pManager)
		{}

		//////////////////////////////////////////////////////////////////////////

		virtual ComponentData *createData(const Entity &entity)
		{
			return entity.getData()->create<InputData>("Input");
		}

		virtual void onStart() {}
		
	protected:
	};
}


#endif //__WORLD_INPUTCOMPONENT_H__
