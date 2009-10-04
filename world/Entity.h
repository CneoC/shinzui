#pragma once

#ifndef __WORLD_ENTITY_H__
#define __WORLD_ENTITY_H__

#include "core/types.h"

#include <boost/thread/mutex.hpp>
#include <boost/static_assert.hpp>

namespace world
{
	typedef	u32	EntityId;

#define ENTITY_NONE	0

	class Entity
	{
	public:
		Entity()
			: m_id(ENTITY_NONE)
		{}

		u32 getId() const	{ return m_id; }

	protected:
		EntityId	m_id;

		//////////////////////////////////////////////////////////////////////////

	public:
		static u32 getNextGUID()
		{
			boost::lock_guard<boost::mutex> lock(ms_guidMutex);
			return ms_guid++;
		}

	private:
		static EntityId		ms_guid;
		static boost::mutex	ms_guidMutex;
	};

	// The entity class is just an identifier with some utilities
	BOOST_STATIC_ASSERT(sizeof(Entity) == 4);
}

#endif //__WORLD_ENTITY_H__
