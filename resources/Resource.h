#pragma once

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "core/types.h"
#include "resources/ResourceTypes.h"

#include <assert.h>

class ResourceLoaderBase;

class ResourceData
{
public:
	enum { TYPE = RESOURCE_NULL };

	enum Flags
	{
		FLAG_LOADED,
		FLAG_LOAD,
		FLAG_UNLOAD,

		FLAG_NEXT
	};

	//////////////////////////////////////////////////////////////////////////

public:
	ResourceData(ResourceLoaderBase *pLoader = NULL)
		: m_references(0)
		, m_pLoader(pLoader)
	{
		setType(TYPE);
	}

	//////////////////////////////////////////////////////////////////////////

	bool isType(u32 type) const				{ return m_type[type]; }

	bool isLoaded() const					{ return m_flags[FLAG_LOADED]; }
	void setLoaded(bool loaded)				{ m_flags[FLAG_LOADED] = loaded; }

	bool isLoading() const					{ return m_flags[FLAG_LOAD]; }
	void setLoad(bool load)					{ m_flags[FLAG_LOAD] = load; }

	bool isUnloading() const				{ return m_flags[FLAG_UNLOAD]; }
	void setUnload(bool unload)				{ m_flags[FLAG_UNLOAD] = unload; }

	u32 getRefCount() const					{ return m_references; }
	void incRefCount()						{ m_references++; }
	void decRefCount()						{ assert(m_references > 0); m_references--; }

	ResourceLoaderBase *getLoader() const	{ return m_pLoader; }

	const std::string &getId() const		{ return m_id; }
	void setId(const std::string &id)		{ m_id = id; }

	//////////////////////////////////////////////////////////////////////////

protected:
	void setType(u32 type)					{ m_type[type] = true; }

	//////////////////////////////////////////////////////////////////////////

protected:
	ResourceTypeSet		m_type;

	std::bitset<16>		m_flags;
	u32					m_references;

	ResourceLoaderBase*	m_pLoader;

	std::string			m_id;
};

template <class T>
class ResourceRef
{
public:
	ResourceRef()
		: m_pData(NULL)
	{
	}

	ResourceRef(T* pData)
		: m_pData(pData)
	{
		assert(m_pData);
		m_pData->incRefCount();
	}

	template <class OT>
	ResourceRef(const ResourceRef<OT>& cast)
	{
		if (cast && (cast.isType(T::TYPE)) == 0)
		{
			throw std::runtime_error("Unable to cast resource to different type.");
		}
		m_pData = static_cast<T *>(cast.getData());
		reference();
	}

	ResourceRef(const ResourceRef& copy)
		: m_pData(copy.m_pData)
	{
		reference();
	}

	~ResourceRef()
	{
		dereference();
	}

	//////////////////////////////////////////////////////////////////////////

	ResourceRef &operator = (const ResourceRef &copy)
	{
		dereference();
		m_pData = copy.m_pData;
		reference();
		return *this;
	}

	T* operator -> ()	{ return m_pData; }

	operator bool () const		{ return m_pData != NULL; }
	
	//////////////////////////////////////////////////////////////////////////

	bool load(u32 flags = 0)
	{
		return	m_pData &&  m_pData->getLoader()->load(ResourceRef<ResourceData>(*this), flags);
	}

	void unload(u32 flags = 0)
	{
		return	m_pData &&  m_pData->getLoader()->unload(ResourceRef<ResourceData>(*this), flags);
	}

	//////////////////////////////////////////////////////////////////////////

	bool isType(u32 type) const	{ return m_pData && m_pData->isType(type); }
	T* getData() const			{ return m_pData; }


protected:
	void reference()
	{
		if (m_pData)
			m_pData->incRefCount();
	}

	void dereference()
	{
		if (m_pData)
		{
			m_pData->decRefCount();
			if (m_pData->getRefCount() == 0)
				delete m_pData;
		}
	}

	T*	m_pData;
};

typedef ResourceRef<ResourceData>	Resource;

#endif //__RESOURCE_H__
