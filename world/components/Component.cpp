#include "Component.h"

#include "world/Entity.h"

using namespace world;

//////////////////////////////////////////////////////////////////////////

Component::Component(core::Core *pCore, ComponentManager *pManager)
	: core::Process(pCore)
	, m_pManager(pManager)
{
	m_color = math::Color3f(0, 1, 1);
}

//////////////////////////////////////////////////////////////////////////

void Component::addEntity(const Entity &entity)
{
	ComponentData *pData = createData(entity);
	ComponentEntry &entry = m_entities[entity.getId()];
	entry.data		= createData(entity);
	entry.dataSet	= entity.getData();
}

void Component::removeEntity(const Entity &entity)
{
	EntityList::iterator found = m_entities.find(entity.getId());
	if (found != m_entities.end())
		m_entities.erase(found);
}
