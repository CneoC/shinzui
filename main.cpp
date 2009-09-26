#include <cstdio>

#include "core/Core.h"
//#include "util/LogTestProc.h"
#include "core/gl/GLContext.h"
#include "render/2d/Console.h"
#include "render/2d/DrawFPS.h"
#include "render/scene/Scene.h"
#include "render/StartRenderer.h"
#include "render/EndRenderer.h"
#include "render/StartFrameBuffer.h"
#include "render/EndFrameBuffer.h"

#include "resources/ResourceLoader.h"
#include "resources/file/FSResourceLoader.h"
#include "resources/font/FontResource.h"

void main(const char *argc, int argv)
{
	try
	{
		Core *pCore = new Core();

		/*
		ResourceCache*	pCache	= new ResourceCache(pCore);
		pCore->addProcess(pCache);

		ResourceLoader*	pLoader	= new ResourceLoader(pCache);
		pCore->setLoader(pLoader);

		pLoader->addLoader(new FSResourceLoader(ResourceTypeSet().set(), "./data/"));

		FontResource debugFont = pLoader->getFont("2d/fonts/debug.ttf");
		ShaderResource bloomFragShader = pLoader->getShader("2d/shaders/bloom.frag");
		ShaderResource bloomVertShader = pLoader->getShader("2d/shaders/bloom.vert");

		debugFont.load(ResourceLoader::FLAG_ASYNC | ResourceLoader::ASYNC_PRIORITY_NORMAL);
		bloomFragShader.load();
		bloomVertShader.load();

		/*
		LogTestProc t1(0, 5.00);
		LogTestProc t2(1, 2.00);
		LogTestProc t3(2, 1.00);
		LogTestProc t4(3, 0.50);
		LogTestProc t5(4, 0.20);

		t5.addDependency(&t1);

		core.addProcess(&t1);
		core.addProcess(&t2);
		core.addProcess(&t3);
		core.addProcess(&t4);
		core.addProcess(&t5);
		*/

		// Create window before doing any other rendering related stuff
		Window		*pWindow		= new Window(pCore);
		pWindow->setTitle("Engine");
		pWindow->setSize(Vector2i(1024, 768));
		pWindow->create();

		GLContext	*pMainContext	= new GLContext(pWindow);
		GLContext	*pLoaderContext	= new GLContext(pWindow);

		pMainContext->create();
		StartRenderer		*pRenderStart	= new StartRenderer(pCore, pMainContext);
		StartFrameBuffer	*pStartFBO		= new StartFrameBuffer(pCore);
		Scene				*pScene			= new Scene(pCore);
		EndFrameBuffer		*pEndFBO		= new EndFrameBuffer(pCore);
		Console				*pConsole		= new Console(pCore);
		DrawFPS				*pFps			= new DrawFPS(pCore);
		EndRenderer			*pRenderEnd		= new EndRenderer(pCore, pMainContext);

		pLoaderContext->create();

		// Add the window process
		pCore->addProcess(pWindow);

		// Add the render process chain
		pCore->addProcess(
			pRenderStart
			->link(pStartFBO)
			->link(pScene)
			->link(pEndFBO)
			->link(pConsole)
			->link(pFps)
			->link(pRenderEnd)
			->link(pRenderStart)
			);

		pWindow->show();

		// Run process handling
		pCore->run();

		delete pCore;

		delete pRenderEnd;
		delete pFps;
		delete pConsole;
		delete pEndFBO;
		delete pScene;
		delete pStartFBO;
		delete pRenderStart;

		// Clean up
		pMainContext->destroy();
		pLoaderContext->destroy();
		delete pMainContext;
		delete pLoaderContext;

		pWindow->destroy();
		delete pWindow;
	}
	catch (std::runtime_error &e)
	{
		printf("Oops: %s\n", e.what());
	}

	return;
}