#include "MoveComponent.h"

#include "world/Entity.h"

using namespace world;

//////////////////////////////////////////////////////////////////////////

MoveComponent::MoveComponent(core::Core *pCore, ComponentManager *pManager)
	: Component(pCore, pManager)
{
	m_transformHash = util::hashString("Transform", 9);
	m_color = math::Color3f(1, 0, 1);

	setFrameDelay(0.0167);
}

//////////////////////////////////////////////////////////////////////////

ComponentData *MoveComponent::createData(const Entity &entity)
{
	entity.getData()->create<TransformData>("Transform");
	return entity.getData()->create<MoveData>("Move");
}

void MoveComponent::onStart()
{
	m_entity = m_entities.begin();
	m_pCore->addJob(this, core::Job::Function(this, &MoveComponent::step));
	m_pCore->addJob(this, core::Job::Function(this, &MoveComponent::step));
	m_pCore->addJob(this, core::Job::Function(this, &MoveComponent::step));
	m_pCore->addJob(this, core::Job::Function(this, &MoveComponent::step));
	m_pCore->addJob(this, core::Job::Function(this, &MoveComponent::step));
	m_pCore->addJob(this, core::Job::Function(this, &MoveComponent::step));
	m_pCore->addJob(this, core::Job::Function(this, &MoveComponent::step));
	m_pCore->addJob(this, core::Job::Function(this, &MoveComponent::step));
}

bool MoveComponent::step()
{
	if (m_entity == m_entities.end())
		return false;

	const double delta = getDeltaTime();

	EntityList::iterator begin;
	EntityList::iterator end;

	{
		boost::lock_guard<boost::mutex> lock(m_mutex);

		begin = m_entity;
		u32 amount = 200;
		while (m_entity != m_entities.end() && amount > 0)
		{
			--amount;
			++m_entity;
		}
		end = m_entity;
	}

	for (EntityList::iterator entry = begin; entry != end; ++entry)
	{
		TransformData *	pTransform	= entry->second.dataSet->get<TransformData>(m_transformHash);
		MoveData *		pMove		= static_cast<MoveData *>(entry->second.data);

		u32 count = 0;
		math::Vector3f addVelocity(0, 0, 0);
		for (EntityList::iterator iter = m_entities.begin(); iter != m_entities.end(); ++iter)
		{
			if (iter->first == entry->first)
				continue;
			TransformData *	pOtherTransform	= iter->second.dataSet->get<TransformData>(m_transformHash);
			math::Vector3f dir(pOtherTransform->position - pTransform->position);
			float dist;
			dir.normalize(dist);
			dist -= 10;
			if (dist > 20)
			{
				addVelocity += dir * (1000 / dist);
				count++;
			}
 			else if (dist < -5)
 			{
				addVelocity += dir * dist;
 				count++;
 			}
		}
		addVelocity /= count;

		pTransform->position += pMove->velocity * delta;
		pMove->velocity	+= (pMove->gravity + addVelocity) * delta;
		pMove->velocity.x  *= powf(pMove->damping.x, delta);
		pMove->velocity.y  *= powf(pMove->damping.y, delta);
		pMove->velocity.z  *= powf(pMove->damping.z, delta);
	}

	return true;
}
