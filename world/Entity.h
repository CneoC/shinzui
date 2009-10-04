#pragma once

#ifndef __WORLD_ENTITY_H__
#define __WORLD_ENTITY_H__

#include "core/types.h"

#include "world/components/ComponentDataSet.h"

#include <boost/thread/mutex.hpp>
#include <boost/static_assert.hpp>

namespace world
{
	class Component;
	class ComponentManager;

#define ENTITY_NONE	0

	class Entity
	{
	public:
		//! NULL entity constructor
		Entity()
			: m_id(ENTITY_NONE)
			, m_pManager(NULL)
		{}

		//! Default entity constructor
		Entity(u32 id, ComponentManager *pManager)
			: m_id(id)
			, m_pManager(pManager)
			, m_data(new ComponentDataSet)
		{}

		//! Copy constructor
		Entity(const Entity &copy)
			: m_id(copy.m_id)
			, m_pManager(copy.m_pManager)
			, m_data(copy.m_data)
		{}

		static Entity create(const std::string &name, ComponentManager *pManager)
		{
			Entity entity(getNextGUID(), pManager);
			//NameData *pData = addComponent("Name");
			//pData->name = name;
			return entity;
		}

		//////////////////////////////////////////////////////////////////////////
		
		const ComponentDataSetRef &getData() const	{ return m_data; }
		ComponentDataSetRef getData()				{ return m_data; }

		void addComponent(const std::string &name);
		void removeComponent(const std::string &name);

		u32 getId() const	{ return m_id; }

	protected:
		u32					m_id;
		ComponentManager *	m_pManager;
		ComponentDataSetRef	m_data;

		//////////////////////////////////////////////////////////////////////////

	public:
		static u32 getNextGUID()
		{
			boost::lock_guard<boost::mutex> lock(ms_guidMutex);
			return ms_guid++;
		}

	private:
		static u32			ms_guid;
		static boost::mutex	ms_guidMutex;
	};
}

#endif //__WORLD_ENTITY_H__
