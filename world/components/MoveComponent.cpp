#include "MoveComponent.h"

#include "world/Entity.h"

using namespace world;

//////////////////////////////////////////////////////////////////////////

MoveComponent::MoveComponent(core::Core *pCore, ComponentManager *pManager)
	: Component(pCore, pManager)
{
	setJobs(64);
	m_transformHash = util::hashString("Transform", 9);
}

//////////////////////////////////////////////////////////////////////////

ComponentData *MoveComponent::createData(const Entity &entity)
{
	entity.getData()->create<TransformData>("Transform");
	return entity.getData()->create<MoveData>("Move");
}

void MoveComponent::start(double delta)
{
	m_entity = m_entities.begin();
}

bool MoveComponent::step(double delta)
{
	ComponentEntry entry;
	{
		boost::lock_guard<boost::mutex> lock(m_mutex);
		if (m_entity == m_entities.end())
			return false;

		entry = m_entity->second;
		++m_entity;
	}

	TransformData *	pTransform	= entry.dataSet->get<TransformData>(m_transformHash);
	MoveData *		pMove		= static_cast<MoveData *>(entry.data);

	pTransform->position += pMove->velocity * delta;
	pMove->velocity	+= pMove->gravity;
	pMove->velocity *= pMove->damping;

	return true;
}
