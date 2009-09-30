#pragma once

#ifndef __RESOURCE_RESOURCETYPE_H__
#define __RESOURCE_RESOURCETYPE_H__

#include "util/Util.h"

#include <string>

// TODO: Optimize to use hashes instead of string find.
class ResourceType
{
public:
	/**
	 * Constructs a NULL resource type.
	 */
	ResourceType()
//		: m_hash(0)
	{
	}

	/**
	 * Constructs a resource type from a type name.
	 */
	ResourceType(const std::string &type)
		: m_types("." + type + ".")
//		, m_hash(0)
	{
	}

	/**
	 * Copies a resource type.
	 */
	ResourceType(const ResourceType &type)
		: m_types(type.m_types)
//		, m_hash(0)
	{
	}

	//////////////////////////////////////////////////////////////////////////

	/**
	 * Append type.
	 */
	const ResourceType & operator += (const std::string &type)
	{
		if (!type.empty())
		{
			m_types += type;
			m_types.push_back('.');
		}
		return *this;
	}
	ResourceType operator + (const std::string &type) const
	{
		ResourceType result(m_types);
		result += type;
		return result;
	}

	/**
	 * Checks if the resource type is valid.
	 */
	operator bool () const	{ return !m_types.empty(); }

	/**
	 * Checks if two types are identical.
	 */
	bool operator == (const ResourceType &other) const
	{
		return m_types == other.m_types;
	}

	/**
	 * Checks if the other type is partially present in this type.
	 */
	bool operator & (const ResourceType &other) const
	{
		return other && m_types.find(other.m_types) != std::string::npos;
	}
	bool operator & (const char *type) const
	{
		return type && m_types.find(std::string(".") + type + '.') != std::string::npos;
	}

	/**
	 * Convert resource id to a string.
	 * @return the name of the identifier.
	 */
	const std::string &toString() const
	{
		return m_types;
	}

	/**
	 * Return the actual (last) type.
	 */
	std::string getTop() const
	{
		std::string result = "";
		if (!m_types.empty())
		{
			u32 pos = m_types.find_last_of('.', m_types.length() - 2);
			if (pos != std::string::npos)
			{
				pos++;
				result = m_types.substr(pos, m_types.length() - pos - 1);
			}
		}
		return result;
	}

	//////////////////////////////////////////////////////////////////////////

	//! Checks if the type is empty
	bool isEmpty() const	{ return m_types.empty(); }


protected:
//	std::vector<u16>			m_hashes;
//	std::vector<std::string>	m_types;
	std::string			m_types;
};

/*
#include <bitset>

enum ResourceType
{
	RESOURCE_NULL,

	// Raw types
	RESOURCE_RAW,
	RESOURCE_FILE,
	RESOURCE_MEMORY,
	
	// Font types
	RESOURCE_FONT,
	RESOURCE_FT_FONT,				// Freetype 2 font
	RESOURCE_TEX_FONT,				// Texture based font
	RESOURCE_GL_FONT,				// OpenGL font

	// Texture types
	RESOURCE_TEXTURE,
	RESOURCE_FI_TEXTURE,			// FreeImage based texture
	RESOURCE_GL_TEXTURE,			// OpenGL texture

	// Shader types
	RESOURCE_SHADER,
	RESOURCE_GL_SHADER,				// OpenGL shader
	RESOURCE_GL_FRAG_SHADER,		// OpenGL fragment shader
	RESOURCE_GL_VERT_SHADER,		// OpenGL vertex shader
	RESOURCE_GL_PROGRAM,			// OpenGL program
	RESOURCE_GL_PROGRAM_DEFINITION, // OpenGL program definition

	NUM_RESOURCE_TYPES
};

typedef std::bitset<NUM_RESOURCE_TYPES>	ResourceTypeSet;
*/

#endif //__RESOURCE_RESOURCETYPE_H__
