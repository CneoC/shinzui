#pragma once

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "core/types.h"
#include "resources/ResourceId.h"
#include "resources/ResourceType.h"

#include <assert.h>
#include <bitset>

#include <boost/thread/mutex.hpp>

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
			m_pData = new T(src->getLoader());
			m_pData->setId(id);
			reference();

			// Let loader handle the rest of the cloning
			*this = pLoader->clone(src, *this);
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
		reference();
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
			// C-style cast works better here, due to it's dynamic nature it will
			// better determine cast types depending on T and OT template types.
			// A reinterpret_cast will break when data objects have virtual tables,
			// and static_cast won't work on different typed resource conversions
			// (even though resource conversions never end up in here).
 			m_pData = (T *)cast.getData();
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
		if (m_pData != copy.m_pData)
		{
			m_pData = copy.m_pData;
			reference();
		}
		return *this;
	}

	bool operator < (const ResourceRef &other) const
	{
		if (!other || !*this)
			return false;
		// Higher priority is more important so should come earlier
		return (*this)->getLoadPriority() > other->getLoadPriority();
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

	/**
	 * Reload the resource
	 * @param flags		see ResourceLoaderBase::LoadFlags
	 */
	bool reload(u32 flags = 0)
	{
		return unload(flags) && load(flags);
	}

	//! Safe check to see if the data is loaded.
	bool isLoaded() const	{ return m_pData && m_pData->isLoaded(); }

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
	: boost::noncopyable
{
public:
	enum Flags
	{
		FLAG_REQUIRE_CONTEXT,
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
		, m_loadPriority(0)
	{
	}

	//////////////////////////////////////////////////////////////////////////

	//! Gets the resource identifier.
	const ResourceId &getId() const				{ return m_id; }
	//! Sets the resource identifier
	void setId(const ResourceId &id)			{ m_id = id; }

	//! Gets the resource type.
	const ResourceType &getType() const			{ return m_type; }
	ResourceType &getType()						{ return m_type; }

	//! Resets the resource flags.
	void resetFlags()							{ m_flags.reset(); }

	//! Resource is loaded.
	bool isLoaded() const						{ return m_flags[FLAG_LOADED]; }
	//! Set the resource is loaded.
	void setLoaded(bool loaded)					{ m_flags[FLAG_LOADED] = loaded; loaded? setLoad(false): setUnload(false); }

	//! Resource is marked for unloading.
	bool isLoading() const						{ return m_flags[FLAG_LOAD]; }
	//! Mark resource for (async) loading.
	void setLoad(bool load)						{ m_flags[FLAG_LOAD] = load; }

	//! Resource is marked for unloading.
	bool isUnloading() const					{ return m_flags[FLAG_UNLOAD]; }
	//! Mark resource for (async) unloading.
	void setUnload(bool unload)					{ m_flags[FLAG_UNLOAD] = unload; }

	//! Resource requires a context to be able to load
	bool requiresContext() const				{ return m_flags[FLAG_REQUIRE_CONTEXT]; }

	//! Gets the reference count for this resource data.
	u32 getRefCount() const						{ return m_references; }
	
	/**
	 * Increment resource data reference count (tread-safe).
	 */
	void incRefCount()
	{
		boost::lock_guard<boost::mutex> lock(m_rcMutex);
		m_references++; 
	}

	/**
	 * Decrement resource data reference count (tread-safe).
	 */
	void decRefCount()
	{ 
		boost::lock_guard<boost::mutex> lock(m_rcMutex);
		assert(m_references > 0); m_references--;
	}

	//! Gets the resource this one is converted from (can be a NULL resource).
	Resource getSource() const					{ return m_source; }
	//! Sets the resource this one is converted from.
	void setSource(const Resource &source)		{ m_source = source; }

	//! Get the loader responsible for this resource.
	ResourceLoaderBase *getLoader() const		{ return m_pLoader; }

	//! Gets the priority this resource has when loading asynchronous.
	s32 getLoadPriority() const					{ return m_loadPriority; }
	//! Sets the priority this resource has for asynchronous loading, higher is more important.
	void setLoadPriority(s32 priority)			{ m_loadPriority = (s16)priority; }

protected:
	// TODO: Pack this data more optimally

	std::bitset<16>		m_flags;		// State flags.
	s16					m_loadPriority;

	ResourceId			m_id;			// Resource identifier.
	ResourceType		m_type;			// Resource types.

	volatile u32		m_references;	// Number of references to this resource.

	Resource			m_source;		// Source data for this resource.
	ResourceLoaderBase*	m_pLoader;		// Loader responsible for this resource.

	boost::mutex		m_rcMutex;		// Mutex used to keep reference counting thread-safe.
};

#endif //__RESOURCE_H__
