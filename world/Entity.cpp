#include "Entity.h"

using namespace world;

//////////////////////////////////////////////////////////////////////////

u32				Entity::ms_guid		= ENTITY_NONE + 1;
boost::mutex	Entity::ms_guidMutex;
