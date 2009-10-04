#pragma once

#ifndef __WORLD_TRANSFORMDATA_H__
#define __WORLD_TRANSFORMDATA_H__

#include "world/components/Component.h"

#include "math/Vector3.h"

namespace world
{
	struct TransformData
		: public ComponentData
	{
		math::Vector3f	position;
		math::Vector3f	scale;
		math::Vector3f	rotation;
	};
}


#endif //__WORLD_TRANSFORMDATA_H__
