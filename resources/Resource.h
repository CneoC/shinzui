#pragma once

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "resources/ResourceTypes.h"

class ResourceData
{
public:
	enum { TYPE = RESOURCE_NULL };

	ResourceData(u32 type)
		: m_type(type)
		, m_references(0)
	{
	}

	u32 getType() const { return m_type; }


	void incRefCount()		{ m_references++; }
	void decRefCount()		{ assert(m_references > 0); m_references--; }
	u32 getRefCount() const	{ return m_references; }

protected:
	u32 m_type;
	u32	m_references;
};

template <class T>
class ResourceRef
{
public:
	ResourceRef()
		: m_pData(NULL)
	{
	}

	ResourceRef(T *pData)
		: m_pData(pData)
	{
		assert(m_pData);
		m_pData->incRefCount();
	}

	template <class OT>
	ResourceRef(const ResourceRef<OT> &cast)
	{
		if ((OT::TYPE & cast.getType()) == 0)
		{
			throw std::runtime_error("Unable to cast resource to different type.");
		}
		m_pData = static_cast<T *>(cast.getData());
	}

	ResourceRef(const ResourceRef &copy)
		: m_pData(copy.m_pData)
	{
		if (m_pData) m_pData->incRefCount();
	}

	~ResourceRef()
	{
		if (m_pData)
		{
			m_pData->decRefCount();
			if (m_pData->getRefCount() == 0) delete m_pData;
		}
	}

	u32 getType() const	{ return m_pData ? m_pData->getType() : RESOURCE_NULL; }
	T *getData() const	{ return m_pData; }

	operator bool () const
	{
		return m_pData != NULL;
	}

protected:
	T *m_pData;
};

typedef ResourceRef<ResourceData>	Resource;

#endif //__RESOURCE_H__
