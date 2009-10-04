#pragma once

#ifndef __RESOURCES_FRAMEBUFFERRESOURCE_H__
#define __RESOURCES_FRAMEBUFFERRESOURCE_H__

#include "TextureResource.h"

#include "math/Color4.h"

//////////////////////////////////////////////////////////////////////////

class FrameBufferDataDef
	: public ResourceData
{
public:
	enum Flags
	{
		FLAG_FULLSCREEN	= ResourceData::FLAG_NEXT,	// Use game resolution as size
		FLAG_CLEAR,									// Clear buffer color on bind
		FLAG_CLEAR_DEPTH,							// Clear buffer depth on bind
		FLAG_ATTACH_DEPTH,							// Attach depth buffer
	};

	static const char *getName()	{ return "FrameBufferDef"; }

	FrameBufferDataDef(ResourceLoaderBase *pData)
		: ResourceData(pData)
	{
		getType() += getName();
	}

	bool isFullscreen() const					{ return m_flags[FLAG_FULLSCREEN]; }
	void setFullscreen(bool fullscreen)			{ m_flags[FLAG_FULLSCREEN] = fullscreen; }

	bool isCleared() const						{ return m_flags[FLAG_CLEAR]; }
	void setCleared(bool clear)					{ m_flags[FLAG_CLEAR] = clear; }

	bool isDepthCleared() const					{ return m_flags[FLAG_CLEAR_DEPTH]; }
	void setDepthCleared(bool clear)			{ m_flags[FLAG_CLEAR_DEPTH] = clear; }

	bool isDepthAttached() const				{ return m_flags[FLAG_ATTACH_DEPTH]; }
	void setDepthAttached(bool attach)			{ m_flags[FLAG_ATTACH_DEPTH] = attach; }

	//! Gets the bytes per pixel for this frame buffer.
	u8 getBpp() const							{ return m_bpp; }
	//! Sets the bytes per pixel for this frame buffer (3=RGB 4=RGBA).
	void setBpp(u8 bpp)							{ m_bpp = bpp; }

	//! Gets the size for the frame buffer.
	const math::Vector2i &getSize() const		{ return m_size; }
	//! Sets the size for the frame buffer.
	void setSize(const math::Vector2i &size)	{ m_size = size; }

	//! Gets the clear color for the frame buffer.
	const math::Color4f &getClearColor() const			{ return m_clearColor; }
	//! Sets the clear color for the frame buffer.
	void setClearColor(const math::Color4f &clearColor)	{ m_clearColor = clearColor; }

protected:
	u8				m_bpp;
	math::Vector2i	m_size;
	math::Color4f	m_clearColor;

};

typedef ResourceRef<FrameBufferDataDef>		FrameBufferDef;

//////////////////////////////////////////////////////////////////////////

class FrameBufferData
	: public TextureData
{
public:
	enum Flags
	{
		FLAG_FULLSCREEN,	// Use game resolution as size
		FLAG_CLEAR,			// Clear buffer on bind
		FLAG_ATTACH_DEPTH,	// Attach depth buffer
	};

	static const char *getName()	{ return "FrameBuffer"; }

	FrameBufferData(ResourceLoaderBase *pData)
		: TextureData(pData)
	{
		getType() += getName();
	}

	TextureResource getTexture() const			{ return m_texture; }
	void setTexture(TextureResource texture)	{ m_texture = texture; }

	FrameBufferDef getDef() const				{ return FrameBufferDef(getSource(), DONT_CONVERT); }

protected:
	TextureResource m_texture;
};

typedef ResourceRef<FrameBufferData>		FrameBufferResource;

#endif //__RESOURCES_FRAMEBUFFERRESOURCE_H__