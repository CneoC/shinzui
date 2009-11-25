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
// Cvar.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#ifndef __CONSOLE_CVAR_H__
#define __CONSOLE_CVAR_H__

#include "core/types.h"

#include "CvarList.h"

#include "math/Color3.h"
#include "math/Color4.h"
#include "math/Vector2.h"
#include "math/Vector3.h"

#include <boost/variant.hpp>

#include <string>
#include <bitset>

namespace console
{
	class CvarList;

	/**
	 * Flags a cvar can have.
	 */
	enum Flags
	{
		CVAR_SAVE,			// save when serializing group
		CVAR_LOCK,			// locked, can never change
		CVAR_LATCH,			// locked, changes will be reflected after unlatch
		CVAR_LATCH_DIRTY,	// value changed during latch, change pending
		CVAR_CHANGED,		// value changed since last save

		NUM_CVAR_FLAGS
	};

	/**
	 * List of supported types used to identify type of cvar.
	 * This list has a direct relation to the list of template arguments
	 * as defined in the ValueType definition!
	 */
	enum Type
	{
		CVAR_INT,
		CVAR_FLOAT,
		CVAR_DOUBLE,
		CVAR_STRING,
		CVAR_BOOL,
		CVAR_VECTOR2,
		CVAR_VECTOR3,
		CVAR_COLOR3,
		CVAR_COLOR4,

		NUM_CVAR_TYPES
	};

	/**
	 * Cvar base class.
	 * Represents a configuration variable.
	 */
	class CvarDef
	{
	public:
		//! Variant type with all possible types a cvar can contain.
		typedef boost::variant<
			int,
			float,
			double,
			std::string,
			bool,
			math::Vector2f,
			math::Vector3f,
			math::Color3f,
			math::Color4f
			> ValueType;

		template < class T >
		CvarDef(const std::string &name, T def, u32 flags = 0)
			: m_name(name)
			, m_hash(util::hashString(name))
			, m_flags(flags)
			, m_value(def)
			, m_latchedValue()
			, m_defaultValue(def)
		{
			CvarList::instance().add(this);
		}

		~CvarDef()
		{
			CvarList::instance().remove(this);
		}

		/**
		 * Latches the cvar.
		 * New values will be saved in m_latchedValue, the actual cvar will only be
		 * overwritten once unlatch is called.
		 */
		void latch();

		/**
		 * Unlatches the cvar.
		 * m_value will be overwritten with m_latchedValue if the cvar has been changed
		 * during it being latched.
		 */
		void unlatch();

		/**
		 * Resets the cvar to it's default value.
		 */
		void reset();

		/**
		 * Gets the value of the cvar as a specific type.
		 * @throws boost::bad_get on failure.
		 */
		template < class T >
		const T &as() const					{ return boost::get<T>(m_value); }

		/**
		 * Gets the value of the cvar as a specific type.
		 * @throws std::bad_cast on failure.
		 */
		template < class T >
		void set(const T &v) const
		{
			ValueType newValue(v);
			if (newValue.which() != m_value.which())
				throw new std::bad_cast("Unable to convert cvar types.");
			m_value = newValue;
		}

		//////////////////////////////////////////////////////////////////////////

		//! Gets the name of the cvar.
		const std::string &getName() const	{ return m_name; }

		//! Gets the name hash of the cvar.
		u32 getHash() const					{ return m_hash; }

		bool isSaved() const				{ return m_flags[CVAR_SAVE]; }
		bool isLocked() const				{ return m_flags[CVAR_LOCK]; }
		bool isLatched() const				{ return m_flags[CVAR_LATCH]; }
		bool isChanged() const				{ return m_flags[CVAR_CHANGED]; }

		//////////////////////////////////////////////////////////////////////////

		//! Gets the type for this Cvar.
		u32 getType()						{ return m_value.which(); }

	protected:
		std::string					m_name;				//!< Name of the cvar.
		u32							m_hash;				//!< Hash of m_name for faster lookup.

		std::bitset<NUM_CVAR_FLAGS>	m_flags;			//!< Active flags for this cvar.

		ValueType					m_value;			//!< Current value of the cvar.
		ValueType					m_latchedValue;		//!< Cache value while the cvar is latched
													//!< m_value is set to this when the cvar is unlatched.
		ValueType					m_defaultValue;		//!< Default value of the cvar used for initial value or reset to default.
	};

	/**
	 * Cvar instance class.
	 * Represents an actual cvar, this allows multiple references to the same cvars
	 * without having to globally share all their instances.
	 */
	class Cvar
	{
	public:
		Cvar(CvarDef &cvar)
			: m_pCvar(&cvar)
		{}

		Cvar(CvarDef *pCvar)
			: m_pCvar(pCvar)
		{}

		Cvar(const std::string &name)
			: m_pCvar(CvarList::instance().find(name))
		{}

		//////////////////////////////////////////////////////////////////////////

		CvarDef & operator -> ()
		{
			return *m_pCvar;
		}

		const CvarDef & operator -> () const
		{
			return *m_pCvar;
		}

		/**
		 * Cast operator.
		 * Tries to cast the instance to the expected type.
		 * @throw boost::bad_get	If unsuccessful
		 */
		template < class T >
		operator const T & () const
		{
			return m_pCvar->as<T>();
		}

		/**
		 * Assignment operator
		 * Tries to set the cvar value.
		 * @param v		The value to set the cvar value to, should be of the same type as the cvar!
		 * @throw std::bad_cast	If unsuccessful
		 */
		template < class T >
		Cvar & operator = (const T &v) const
		{
			return m_pCvar->set(v);
			return *this;
		}

	protected:
		CvarDef *m_pCvar;
	};
}

#define CVAR_DEFINE(ID, ...)	console::CvarDef	ID(__VA_ARGS__)

#define CVAR_INSTANCE(ID)		console::Cvar		NAME

#endif /* __CONSOLE_CVAR_H__ */
