#pragma once

#ifndef __RENDER_GL2DLOADER_H__
#define __RENDER_GL2DLOADER_H__

#include "render/RendererLoaderBase.h"

#include "GLFontUtil.h"

namespace render
{
	class GL2DLoader
		: public RendererLoaderBase
	{
	public:
		GL2DLoader(core::Core *pCore)
			: RendererLoaderBase(pCore)
		{
			addUtil("Font", new GLFontUtil());
		}
	};
}

#endif __RENDER_GL2DLOADER_H__
