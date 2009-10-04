#include "ComponentManager.h"

#include "Component.h"

using namespace world;

ComponentManager::ComponentManager(core::Core *pCore, u32 id, u32 threadMask, u32 jobThreadMask)
	: core::Process(pCore, id, threadMask, jobThreadMask)
{
	m_color = math::Color3f(1, 1, 0);
}

core::Process *ComponentManager::run(u32 job, double delta)
{
// 	ComponentList::iterator component;
// 	for (component = m_components.begin(); component != m_components.end(); ++component)
// 	{
// 		component->second->start(delta);
// 	}
	
	return this;
}
