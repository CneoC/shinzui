//////////////////////////////////////////////////////////////////////////
//
// This file is part of Shinzui.
//
// Shinzui is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shinzui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shinzui.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////
//
// TransformData.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

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
