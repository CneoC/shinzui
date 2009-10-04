#include "GLRenderDriver.h"

#include "render/gl/core/GLStartRenderer.h"
#include "render/gl/core/GLEndRenderer.h"
#include "render/gl/core/GLStartFrameBuffer.h"
#include "render/gl/core/GLEndFrameBuffer.h"

#include "render/gl/2d/GL2DLoader.h"

using namespace render;

//////////////////////////////////////////////////////////////////////////

GLRenderDriver::GLRenderDriver(core::Core *pCore, os::Window *pWindow)
	: RenderDriver(pCore)
	, m_pWindow(pWindow)
{
	m_pContext = new os::GLContext(pWindow);
	getGLContext()->create();

	m_pLoaderContext = new os::GLContext(pWindow);
	getLoaderGLContext()->create();
	getLoaderGLContext()->unbind();

	getGLContext()->link(getLoaderGLContext());

	// Register core renderers
	addRenderer("Start",	&GLStartRenderer::create);
	addRenderer("End",		&GLEndRenderer::create);
	addRenderer("StartFB",	&GLStartFrameBuffer::create);
	addRenderer("EndFB",	&GLEndFrameBuffer::create);

	// Register core utilities

	// Register core loaders
	addLoader("2d", new GL2DLoader(pCore));
}

GLRenderDriver::~GLRenderDriver()
{
	getGLContext()->destroy();
	getLoaderGLContext()->destroy();
}
