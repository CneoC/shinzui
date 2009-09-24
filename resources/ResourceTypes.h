#pragma once

#ifndef __RESOURCE_TYPES_H__
#define __RESOURCE_TYPES_H__

enum ResourceType
{
	RESOURCE_NULL,
	RESOURCE_FONT		= 1 << 0,
	RESOURCE_TEXTURE	= 1 << 1,
	RESOURCE_SHADER		= 1 << 2,
};

#endif //__RESOURCE_TYPES_H__
