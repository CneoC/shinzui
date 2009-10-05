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
