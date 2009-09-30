#pragma once

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "core/types.h"
#include "resources/ResourceId.h"
#include "resources/ResourceType.h"

#include <assert.h>
#include <bitset>

class ResourceLoaderBase;
class ResourceData;

enum DontConvertResource { DONT_CONVERT };

class ResourceData;
template <class T> class ResourceRef;
typedef ResourceRef<ResourceData>	Resource;

/**
 * Reference counter for resource data objects.
 * @template T	Type of resource data
 */
template <class T>
class ResourceRef
{
public:
	/**
	 * Construct a NULL resource
	 */
	ResourceRef()
		: m_pData(NULL)
	{
	}

	/**
	 * Creates a custom user resource without any data.
	 * @param pLoader	The resource loader to use
	 */
	ResourceRef(ResourceLoaderBase *pLoader)
		: m_pData(new T(pLoader))
	{
		reference();
	}

	/**
	 * Loads a resource by name
	 * @param pLoader	The resource loader to use
	 * @param id		The identifier to get.
	 */
	ResourceRef(ResourceLoaderBase *pLoader, const ResourceId &id)
		: m_pData(NULL)
	{
		*this = pLoader->get(id);
	}

	/**
	 * Loads a resource by name and clones it with a new name.
	 * The resource data is not copied, just the definition, so it needs to be reloaded.
	 * @param pLoader	The resource loader to use
	 * @param id		The identifier to get.
	 * @param copy		If the resource was found make a copy of it with this identifier.
	 */
	ResourceRef(ResourceLoaderBase *pLoader, const ResourceId &id, const ResourceId &copy)
		: m_pData(NULL)
	{
		assert(copy);
		ResourceRef<T> src = pLoader->get(id);

		// If resource was loaded clone it
		if (src)
		{
			// Determine new id
			ResourceId id;
			if (copy.getType().isEmpty())
				id = ResourceId(src->getType().getTop(), copy.getName());
			else
				id = copy;

			// Create data with proper id
			m_pData = new T(pLoader);
			m_pData->setId(id);
			reference();

			// Let loader handle cloning
			pLoader->clone(src, *this);
		}
	}

	/**
	 * Construct resource with a resource data object.
	 * @param pData	Data object (ResourceRef takes ownership and will delete it once no references remain!)
	 */
	ResourceRef(T *pData)
		: m_pData(pData)
	{
		assert(m_pData);
		m_pData->incRefCount();
	}

	/**
	 * Copy constructor from different type of resource.
	 */
	template <class OT>
	ResourceRef(const ResourceRef<OT>& cast)
	{
		// Cast compatible resource types
		if (cast && cast.getType() & T::getName())
 		{
 			m_pData = reinterpret_cast<T *>(cast.getData());
 			reference();
 		}
		// Try to convert incompatible resource types
		else if (cast)
		{
			ResourceRef<T> resource = cast->getLoader()->getRoot()->convert(cast, ResourceType(T::getName()));
			m_pData = resource.getData();
			if (m_pData) 
				reference();
		}
		// Otherwise become a NULL resource 
		else
		{
			m_pData = NULL;
		}
	}

	/**
	 * Conversion constructor from different type of resource.
	 */
	template <class OT>
	ResourceRef(const ResourceRef<OT>& cast, DontConvertResource)
	{
		// Cast compatible resource types
		if (cast && cast.getType() & T::getName())
		{
			m_pData = static_cast<T *>(cast.getData());
			reference();
		}
		// Otherwise become a NULL resource 
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
		return m_pData && m_pData->getLoader()->load(ResourceRef<ResourceData>(*this, DONT_CONVERT), flags);
	}

	/**
	 * Unload the resource
	 * @param flags		see ResourceLoaderBase::LoadFlags
	 */
	bool unload(u32 flags = 0)
	{
		return m_pData && m_pData->getLoader()->unload(ResourceRef<ResourceData>(*this, DONT_CONVERT), flags);
	}

	//////////////////////////////////////////////////////////////////////////

	ResourceType getType() const
	{
		if (m_pData)
			return m_pData->getType();
		return ResourceType();
	}

	T *getData() const				{ return m_pData; }

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

/**
 * Base resource data class.
 */
class ResourceData
{
public:
	enum Flags
	{
		FLAG_LOADED,
		FLAG_LOAD,
		FLAG_UNLOAD,

		FLAG_NEXT
	};

	static const char *getName()	{ return "Resource"; }

	//////////////////////////////////////////////////////////////////////////

	ResourceData(ResourceLoaderBase *pLoader = NULL)
		: m_references(0)
		, m_pLoader(pLoader)
		, m_type(getName())
	{
	}

	//////////////////////////////////////////////////////////////////////////

	const ResourceType &getType() const			{ return m_type; }
	ResourceType &getType()						{ return m_type; }

	void resetFlags()							{ m_flags.reset(); }

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

	const ResourceId &getId() const				{ return m_id; }
	void setId(const ResourceId &id)			{ m_id = id; }

protected:

protected:
	std::bitset<16>		m_flags;		// State flags.

	ResourceId			m_id;			// Resource identifier.
	ResourceType		m_type;			// Resource types.

	u32					m_references;	// Number of references to this resource.

	Resource			m_source;		// Source data for this resource.
	ResourceLoaderBase*	m_pLoader;		// Loader responsible for this resource.
};

#endif //__RESOURCE_H__
