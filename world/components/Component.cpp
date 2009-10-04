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

core::Process *Component::run(u32 job, double delta)
{
	if (job == 0)
	{
		boost::lock_guard<boost::shared_mutex> lock(m_startMutex);
		start(delta);
	}
	else
	{
		boost::shared_lock<boost::shared_mutex> lock(m_startMutex);
	}

	while (step(delta)) {}

	return this;
}
