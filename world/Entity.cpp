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