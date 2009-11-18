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
// RendererLoaderBase.h
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RENDER_RENDERERLOADERBASE_H__
#define __RENDER_RENDERERLOADERBASE_H__

#include "core/types.h"

#include "util/Util.h"

#include <string>
#include <map>
#include <list>

namespace core
{
	class Core;
}

namespace render
{
	class RenderUtil;
	class Renderer;

	class RendererLoaderBase
	{
	protected:
		//! Don't allow construction.
		RendererLoaderBase(core::Core *pCore) : m_pCore(pCore) {}

	public:
		typedef Renderer *(*RenderCreateFunc)(core::Core *);

		typedef std::map<u32, RenderCreateFunc>		RendererList;
		typedef std::map<u32, RenderUtil *>			UtilList;
		typedef std::map<u32, RendererLoaderBase *>	LoaderList;

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Clean up renderer.
		 */
		virtual ~RendererLoaderBase();

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Adds a deeper level loader to this loader.
		 * @param name		The name of the loader.
		 * @param pLoader	The loader to add.
		 */
		void addLoader(const std::string &name, RendererLoaderBase *pLoader)
		{
			u32 hash = util::hashString(name.c_str(), name.length());
			m_loaders[hash]	= pLoader;
		}

		/**
		 * Adds a renderer to this loader.
		 * @param name		The name of the renderer.
		 * @param pRenderer	The renderer to add.
		 */
		void addRenderer(const std::string &name, RenderCreateFunc createFunc)
		{
			u32 hash = util::hashString(name.c_str(), name.length());
			m_renderers[hash]	= createFunc;
		}

		/**
		 * Adds a deeper level util to this util.
		 * @param name		The name of the util.
		 * @param pUtil	The util to add.
		 */
		void addUtil(const std::string &name, RenderUtil *pUtil)
		{
			u32 hash = util::hashString(name.c_str(), name.length());
			m_util[hash]	= pUtil;
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Creates an instance of a renderer class.
		 * @param name	The name of the renderer class to create.
		 * @return The new instance of the renderer "name" or NULL if not found.
		 */
		Renderer *createRenderer(const std::string &name) const
		{
			// Looking for deeper level loader
			u32 pos = name.find_first_of('.');
			if (pos != std::string::npos)
			{
				RendererLoaderBase *pLoader = getLoader(name.substr(0, pos - 1));
				if (pLoader)
					return pLoader->createRenderer(name.substr(pos));
				return NULL;
			}

			// Arrived at proper level, so try to create the renderer.
			return createRenderer(util::hashString(name.c_str(), name.length()));
		}

		/**
		 * Gets a utility class instance.
		 * These classes are shared and should have no persistent data!
		 * @param name	The name of the utility class.
		 * @return A shared instance of the utility class "name" or NULL if not found.
		 */
		RenderUtil *getUtil(const std::string &name) const
		{
			// Looking for deeper level utility
			u32 pos = name.find_first_of('.');
			if (pos != std::string::npos)
			{
				RendererLoaderBase *pLoader = getLoader(name.substr(0, pos));
				if (pLoader)
					return pLoader->getUtil(name.substr(pos + 1));
				return NULL;
			}

			// Arrived at proper level, so try to create the utility.
			return getUtil(util::hashString(name.c_str(), name.length()));
		}

	protected:
		/**
		 * Recursive function to create renderer in this or one of the children loaders.
		 * Separate function to prevent generating the hash multiple times.
		 */
		virtual Renderer *createRenderer(u32 hash) const;

		/**
		 * Recursive function to get a util in this or one of the children loaders.
		 * Separate function to prevent generating the hash multiple times.
		 */
		virtual RenderUtil *getUtil(u32 hash) const;

		/**
		 * Finds a loader with a specific name
		 */
		RendererLoaderBase *getLoader(const std::string &name) const;

	protected:
		core::Core *	m_pCore;
		RendererList	m_renderers;
		UtilList		m_util;
		LoaderList		m_loaders;
	};
}

#endif __RENDER_RENDERERLOADERBASE_H__
