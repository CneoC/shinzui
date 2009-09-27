#pragma once

#ifndef __RESOURCE_TYPES_H__
#define __RESOURCE_TYPES_H__

#include <bitset>

enum ResourceType
{
	RESOURCE_NULL,
	
	// Base types
	RESOURCE_FONT,
	RESOURCE_TEXTURE,
	RESOURCE_SHADER,

	// Font types
	RESOURCE_FT_FONT,			// Freetype 2 font
	RESOURCE_TEX_FONT,			// Texture based font
	RESOURCE_GL_FONT,			// OpenGL font

	// Texture types
	RESOURCE_FI_TEXTURE,		// FreeImage based texture
	RESOURCE_GL_TEXTURE,		// OpenGL texture

	// Shader types
	RESOURCE_GL_SHADER,			// OpenGL shader
	RESOURCE_GL_VERT_SHADER,	// OpenGL vertex shader
	RESOURCE_GL_FRAG_SHADER,	// OpenGL fragment shader

	NUM_RESOURCE_TYPES
};

typedef std::bitset<NUM_RESOURCE_TYPES>	ResourceTypeSet;

#endif //__RESOURCE_TYPES_H__
