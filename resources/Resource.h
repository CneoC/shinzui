#pragma once

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "core/types.h"
#include "resources/ResourceTypes.h"

#include <assert.h>

class ResourceLoaderBase;
class ResourceData;

/**
 * Reference counter for resource data objects.
 * @template T	Type of resource data
 */
template <class T>
class ResourceRef
{
public:
	ResourceRef()
		: m_pData(NULL)
	{
	}

	/**
	 * Constructor used for cleaner user created resources.
	 */
	ResourceRef(ResourceLoaderBase *pLoader)
		: m_pData(new T(pLoader))
	{
		reference();
	}

	ResourceRef(T *pData)
		: m_pData(pData)
	{
		assert(m_pData);
		m_pData->incRefCount();
	}

	template <class OT>
	ResourceRef(const ResourceRef<OT>& cast, bool convert = true)
	{
		// Cast compatible resource types
		if (cast && cast.isType(T::TYPE))
		{
			m_pData = reinterpret_cast<T *>(cast.getData());
			reference();
		}
		// Try to convert incompatible resource types
		else if (convert && cast)
		{
			ResourceRef<T> resource = cast->getLoader()->getRoot()->convert(cast, (ResourceType)T::TYPE);
			m_pData = resource.m_pData;
			if (m_pData) 
				reference();
		}
		// Become a NULL resource if 
		else
		{
			m_pData = NULL;
		}
	}

	/**
	 * Copy constructor.
	 */
	ResourceRef(const ResourceRef& copy)
		: m_pData(copy.m_pData)
	{
		reference();
	}

	/**
	 * Destructor
	 */
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

	T *operator -> ()				{ return m_pData; }
	const T *operator -> () const	{ return m_pData; }

	operator bool () const			{ return m_pData != NULL; }

	//////////////////////////////////////////////////////////////////////////

	/**
	 * Load the resource
	 * @param flags		see ResourceLoaderBase::LoadFlags
	 */
	bool load(u32 flags = 0)
	{
		return m_pData && m_pData->getLoader()->load(ResourceRef<ResourceData>(*this), flags);
	}

	/**
	 * Unload the resource
	 * @param flags		see ResourceLoaderBase::LoadFlags
	 */
	bool unload(u32 flags = 0)
	{
		return m_pData && m_pData->getLoader()->unload(ResourceRef<ResourceData>(*this), flags);
	}

	//////////////////////////////////////////////////////////////////////////

	bool isType(u32 type) const	{ return m_pData && m_pData->isType(type); }
	T *getData() const			{ return m_pData; }

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

/**
 * Base resource data class.
 */
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

	bool isType(u32 type) const					{ return m_type[type]; }
	ResourceTypeSet getType() const				{ return m_type; }

	bool isLoaded() const						{ return m_flags[FLAG_LOADED]; }
	void setLoaded(bool loaded)					{ m_flags[FLAG_LOADED] = loaded; loaded? setLoad(false): setUnload(false); }

	bool isLoading() const						{ return m_flags[FLAG_LOAD]; }
	void setLoad(bool load)						{ m_flags[FLAG_LOAD] = load; }

	bool isUnloading() const					{ return m_flags[FLAG_UNLOAD]; }
	void setUnload(bool unload)					{ m_flags[FLAG_UNLOAD] = unload; }

	u32 getRefCount() const						{ return m_references; }
	void incRefCount()							{ m_references++; }
	void decRefCount()							{ assert(m_references > 0); m_references--; }

	Resource getSource() const					{ return m_source; }
	void setSource(const Resource &source)		{ m_source = source; }

	ResourceLoaderBase *getLoader() const		{ return m_pLoader; }

	const std::string &getId() const			{ return m_id; }
	void setId(const std::string &id)			{ m_id = id; }

	//////////////////////////////////////////////////////////////////////////

protected:
	void setType(u32 type)						{ m_type[type] = true; }

	//////////////////////////////////////////////////////////////////////////

protected:
	ResourceTypeSet		m_type;

	std::bitset<16>		m_flags;
	u32					m_references;

	Resource			m_source;
	
	ResourceLoaderBase*	m_pLoader;

	std::string			m_id;
};

#endif //__RESOURCE_H__
