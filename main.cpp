#include <cstdio>

#include "core/Core.h"
#include "util/LogTestProc.h"
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

#define PROCESS_OVERHEAD_TEST 50000

void main(const char *argc, int argv)
{
	try
	{
		logging::Root::getRoot().setWriter(new logging::ConsoleWriter());


		Core *pCore = new Core();

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

#if PROCESS_OVERHEAD_TEST
		LogTestProc **pTestLogs = new LogTestProc *[PROCESS_OVERHEAD_TEST];
		for (u32 i = 0; i < PROCESS_OVERHEAD_TEST; i++)
		{
			LogTestProc *test = new LogTestProc(pCore, i);
			test->setFrameDelay(1 + i * 0.01);
			pCore->addProcess(test);
			pTestLogs[i] = test;
		}
#endif

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

#if PROCESS_OVERHEAD_TEST
		logging::Log &log = logging::Root::getRoot();
		if (LOG_CHECK(log, LEVEL_TRACE))
		{
			for (u32 i = 0; i < PROCESS_OVERHEAD_TEST; i++)
			{
				double diff = pCore->getElapsedTime() - pTestLogs[i]->getNextRunTime();
				if (diff > 0.01) LOG_TRACE(log, "Process " << i << " hasn't been run in " << diff << " seconds.");
			}
		}
		delete pTestLogs;
#endif
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