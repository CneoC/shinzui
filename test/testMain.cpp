#ifdef ENABLE_UNIT_TEST

#include <cstdio>

#include <FreeImagePlus.h>

#include <core/Core.h>

#include <os/current/gl/GLContext.h>

#include <util/console/Console.h>
#include <util/console/DrawFPS.h>

#include <render/gl/GLRenderDriver.h>
#include <render/core/StartFrameBuffer.h>
#include <render/core/EndFrameBuffer.h>
#include <render/RenderChain.h>
#include <render/RenderProc.h>

#include <world/Scene.h>

#include <util/logging/Writer.h>
#include <util/logging/Formatter.h>

#include <resources/ResourceLoader.h>
#include <resources/ResourceCache.h>
#include <resources/loaders/FileResourceLoader.h>
#include <resources/converters/ResourceConverters.h>
#include <resources/font/FontResource.h>
#include <resources/font/GLFontResource.h>
#include <resources/shader/GLProgramResource.h>
#include <resources/texture/GLFrameBufferResource.h>

#include <test/LogTestProc.h>
#include <test/StreamResourceTest.h>

#define ENABLE_RESOURCE_TEST

//#define ENABLE_POST_PROCESSING_TEST

#define ENABLE_ASYNC_LOAD_TEST
#define ASYNC_LOAD_TEST_COUNT		500

#define ENABLE_PROCESS_OVERHEAD_TEST
#define PROCESS_OVERHEAD_TEST_COUNT	10000

void main(const char *argc, int argv)
{
	try
	{
		FreeImage_Initialise();

		logging::TextFormatter	textFormatter;
		logging::FileWriter		fileWriter("game.log");
		logging::ConsoleWriter	consoleWriter;

		fileWriter.setFormatter(&textFormatter);
		consoleWriter.setFormatter(&textFormatter);

		LOG_GET_ROOT->addWriter(&fileWriter);
		LOG_GET_ROOT->addWriter(&consoleWriter);
		consoleWriter.setLevel(LEVEL_WARN);
		
		core::Core *pCore = new core::Core();

		ResourceCache*	pCache	= new ResourceCache(pCore);
		pCore->addProcess(pCache);

		ResourceLoader*	pLoader	= new ResourceLoader(pCache);
		pCore->setLoader(pLoader);

		pLoader->addLoader(new FileResourceLoader("./data/"));
		pLoader->addLoader(new ResourceConverters);

#ifdef ENABLE_RESOURCE_TEST
		FontResource font1(pLoader, "File::2d/fonts/debug.ttf");
		FontResource font2(pLoader, "Font::2d/fonts/debug.ttf");					// == font1
		GLFontResource font3(pLoader, "GLFont::2d/fonts/debug.ttf");				// NULL
		GLFontResource font4(pLoader, "File::2d/fonts/debug.ttf");					// getSource() == font1
		GLFontResource font5(pLoader, "GLFont::2d/fonts/debug.ttf");				// == font4
		GLFontResource font6(pLoader, "GLFont::2d/fonts/debug.ttf", "debug");		// != font4  getId == GLFont::debug
		GLFontResource font7(pLoader, "GLFont::debug");								// == font6
		GLFontResource font8(pLoader, "GLFont::2d/fonts/debug.ttf", "File::debug");	// != font4  getId == File::debug
		GLFontResource font9(pLoader, "File::debug");								// != font7

		GLFontResource font10(font1, DONT_CONVERT);									// font3 == NULL, no automatic resource conversion

		GLFontResource font11(font2);												// == font4

		font4.load(	ResourceLoader::FLAG_ASYNC |									// font2, font1 and file resource (font1->getSource()) marked for loading
					ResourceLoader::ASYNC_PRIORITY_NORMAL);
#endif //ENABLE_RESOURCE_TEST

#ifdef ENABLE_PROCESS_OVERHEAD_TEST
		test::LogTestProc **pTestLogs = new test::LogTestProc *[PROCESS_OVERHEAD_TEST_COUNT];
		for (u32 i = 0; i < PROCESS_OVERHEAD_TEST_COUNT; i++)
		{
			test::LogTestProc *test = new test::LogTestProc(pCore, i);
			test->setFrameDelay(1 + i * 0.01);
			pCore->addProcess(test);
			pTestLogs[i] = test;
		}
#endif //ENABLE_PROCESS_OVERHEAD_TEST

		// Create window before doing any other rendering related stuff
		Window		*pWindow		= new Window(pCore);
		pWindow->setTitle("Engine");
		pWindow->setSize(math::Vector2i(1024, 768));
		pWindow->create();

		render::RenderDriver *pDriver	= new render::GLRenderDriver(pCore, pWindow);
		pCore->setDriver(pDriver);

		pCore->getDriver()->getContext()->bind();

		render::Renderer *	pRenderStart	= pCore->getDriver()->createRenderer("Start");
		render::Renderer *	pStartFB		= pCore->getDriver()->createRenderer("StartFB");
		render::Renderer *	pScene			= new world::Scene(pCore);
		render::Renderer *	pDrawFPS		= new console::DrawFPS(pCore);
		render::Renderer *	pConsole		= new console::Console(pCore);
		render::Renderer *	pEndFB			= pCore->getDriver()->createRenderer("EndFB");
		render::Renderer *	pRenderEnd		= pCore->getDriver()->createRenderer("End");

		render::RenderChain *pChain = new render::RenderChain(pCore);
		pChain->link(pRenderStart)
#ifdef ENABLE_POST_PROCESSING_TEST
 			->link(pStartFB)
			->link(pScene)
 			->link(pEndFB)
#else //!ENABLE_POST_PROCESSING_TEST
			->link(pScene)
#endif //!ENABLE_POST_PROCESSING_TEST
			->link(pConsole)
			->link(pDrawFPS)
			->link(pRenderEnd);

		render::RendererProc *pRenderProc = new render::RendererProc(pCore);
		pRenderProc->setRenderer(pChain);

		pCore->getDriver()->getContext()->unbind();

		pCore->getDriver()->getLoaderContext()->bind();

#ifdef ENABLE_POST_PROCESSING_TEST
		FrameBufferDef fbDef(pLoader);
		fbDef->setClearColor(math::Color4f(0, 0, 0, 0));
		fbDef->setCleared(true);
		fbDef->setDepthCleared(true);
		fbDef->setDepthAttached(true);
		fbDef->setSize(math::Vector2i(1024, 768));
		fbDef->setBpp(4);
		fbDef->setLoaded(true);
		GLFrameBufferResource fb(fbDef);
		fb.load();
		pStartFB->as<render::StartFrameBuffer>()->setFrameBuffer(fb);
		pEndFB->as<render::EndFrameBuffer>()->setFrameBuffer(fb);

		ProgramResource programDef(pLoader);
		programDef->addShader("File::2d/shaders/bloom.frag");
		programDef->addShader("File::2d/shaders/bloom.vert");
		programDef->setLoaded(true);
		GLProgramResource program(programDef);
		program.load(ResourceLoaderBase::FLAG_ASYNC);
		pEndFB->as<render::EndFrameBuffer>()->setProgram(program);
#endif //ENABLE_POST_PROCESSING_TEST

#ifdef ENABLE_ASYNC_LOAD_TEST
		pCore->getLoader()->addLoader(new StreamResourceTestLoader());
		for (u32 i = 0; i < ASYNC_LOAD_TEST_COUNT; i++)
		{
			char pName[32];
			sprintf(pName, "Test::testing_%d", i);

			StreamResourceTestResource test(pCore->getLoader(), pName);
			test->setLoadPriority(-(ASYNC_LOAD_TEST_COUNT / 2) + i);
			test.load(ResourceLoaderBase::FLAG_ASYNC);
		}
#endif //ENABLE_ASYNC_LOAD_TEST

		pCore->getDriver()->getLoaderContext()->unbind();

		// Add the window process
		pCore->addProcess(pWindow);

		// Add the render process chain
		pCore->addProcess(pRenderProc);

		pWindow->show();

		// Run process handling
		pCore->run();

#ifdef ENABLE_PROCESS_OVERHEAD_TEST
		logging::Log *log = LOG_GET_ROOT;
		if (LOG_CHECK(log, LEVEL_TRACE))
		{
			for (u32 i = 0; i < PROCESS_OVERHEAD_TEST_COUNT; i++)
			{
				double diff = pCore->getElapsedTime() - pTestLogs[i]->getNextRunTime();
				if (diff > 0.01) LOG_TRACE(log, "Process " << i << " hasn't been run in " << diff << " seconds.");
			}
		}
		delete pTestLogs;
#endif //ENABLE_PROCESS_OVERHEAD_TEST

		delete pCore;

		pWindow->destroy();
		delete pWindow;

		LOG_GET_ROOT->clearWriters();
	}
	catch (std::runtime_error &e)
	{
		printf("Oops: %s\n", e.what());
	}

	FreeImage_DeInitialise();

	return;
}

#endif //ENABLE_UNIT_TEST