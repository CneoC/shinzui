#pragma once

#ifndef __RENDER_RENDERERLOADERBASE_H__
#define __RENDER_RENDERERLOADERBASE_H__

#include "core/types.h"

#include "util/Util.h"

#include <string>
#include <map>
#include <list>

class Core;

namespace render
{
	class RenderUtil;
	class Renderer;

	class RendererLoaderBase
	{
	protected:
		//! Don't allow construction.
		RendererLoaderBase(Core *pCore) : m_pCore(pCore) {}

	public:
		typedef Renderer *(*RenderCreateFunc)(Core *);

		typedef std::map<u32, RenderCreateFunc>	RendererList;
		typedef std::map<u32, RenderUtil *>		UtilList;
		typedef std::list<RendererLoaderBase *>	LoaderList;

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Clean up renderer.
		 */
		virtual ~RendererLoaderBase();

		//////////////////////////////////////////////////////////////////////////

		/**
		 * Creates an instance of a renderer class.
		 * @param name	The name of the renderer class to create.
		 * @return The new instance of the renderer "name" or NULL if not found.
		 */
		Renderer *createRenderer(const std::string &name) const
		{
			return createRenderer(Util::hashString(name.c_str(), name.length()));
		}

		/**
		 * Gets a utility class instance.
		 * These classes are shared and should have no persistent data!
		 * @param name	The name of the utility class.
		 * @return A shared instance of the utility class "name" or NULL if not found.
		 */
		RenderUtil *getUtil(const std::string &name) const
		{
			return getUtil(Util::hashString(name.c_str(), name.length()));
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

	protected:
		Core *			m_pCore;
		RendererList	m_renderers;
		UtilList		m_utils;
		LoaderList		m_loaders;
	};
}

#endif __RENDER_RENDERERLOADERBASE_H__
