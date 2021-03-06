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
// main.cpp
// Copyright (c) 2009 Coen Campman
//
//////////////////////////////////////////////////////////////////////////

#include <cstdio>

#include <FreeImagePlus.h>

#include <core/Core.h>

#include <os/current/gl/GLContext.h>
#include <os/current/Exception.h>

#include <util/console/Console.h>
#include <util/console/ThreadUsage.h>

#include <render/gl/GLRenderDriver.h>
#include <render/core/StartFrameBuffer.h>
#include <render/core/EndFrameBuffer.h>
#include <render/2d/swf/DrawSWF.h>
#include <render/2d/util/DrawConsole.h>
#include <render/2d/util/DrawFPS.h>
#include <render/2d/util/DrawThreadUsage.h>
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

#include <world/components/ComponentManager.h>
#include <world/components/MoveComponent.h>
#include <world/components/InputComponent.h>
#include <world/components/RenderBoxComponent.h>

#include <script/PythonScriptEngine.h>

#include <test/LogTestProc.h>
#include <test/StreamResourceTest.h>

//#define ENABLE_RESOURCE_TEST
//#define ENABLE_ASYNC_LOAD_TEST
#define PROCESS_OVERHEAD_TEST 0 //10000

using namespace core;
using namespace os;
using namespace logging;
using namespace resources;
using namespace render;

void main(const char *argc, int argv)
{
	try
	{
		FreeImage_Initialise();

		/************************************************************************/
		/* Initialize log system                                                */
		/************************************************************************/
		TextFormatter	textFormatter;
		FileWriter		fileWriter("game.log");
		ConsoleWriter	consoleWriter;

		fileWriter.setFormatter(&textFormatter);
		consoleWriter.setFormatter(&textFormatter);

		LOG_GET_ROOT->addWriter(&fileWriter);
		LOG_GET_ROOT->addWriter(&consoleWriter);
		consoleWriter.setLevel(LEVEL_INFO);

		/************************************************************************/
		/* Initialize core and resource system                                  */
		/************************************************************************/
		Core *pCore = new Core();

		ResourceCache*	pCache	= new ResourceCache(pCore);
		pCache->setLoadJobs(1 + (pCore->getThreadCount() / 2));
		pCore->addProcess(pCache);

		ResourceLoader*	pLoader	= new ResourceLoader(pCache);
		pCore->setLoader(pLoader);

		pLoader->addLoader(new loaders::FileResourceLoader("./data/"));
		pLoader->addLoader(new converters::ResourceConverters);

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
#endif

#if PROCESS_OVERHEAD_TEST
		test::LogTestProc **pTestLogs = new test::LogTestProc *[PROCESS_OVERHEAD_TEST];
		for (u32 i = 0; i < PROCESS_OVERHEAD_TEST; i++)
		{
			test::LogTestProc *test = new LogTestProc(pCore, i);
			test->setDelay(1 + i * 0.01);
			pCore->addProcess(test);
			pTestLogs[i] = test;
		}
#endif

		/************************************************************************/
		/* Console system initialization                                        */
		/************************************************************************/
		console::ThreadUsage	threadUsage(pCore);
		console::Console		console(pCore);

		threadUsage.start();

		console::GameConsoleWriter	gameConsoleWriter(console);
		gameConsoleWriter.setFormatter(&textFormatter);
		LOG_GET_ROOT->addWriter(&gameConsoleWriter);
		gameConsoleWriter.setLevel(LEVEL_INFO);

		/************************************************************************/
		/* Window creation                                                      */
		/************************************************************************/
		Window		*pWindow		= new Window(pCore);
		pWindow->setTitle("Game");
		pWindow->setSize(math::Vector2i(1024, 768));
		pWindow->create();
		pCore->addProcess(pWindow);

		RenderDriver *pDriver	= new GLRenderDriver(pCore, pWindow);
		pCore->setDriver(pDriver);

		/************************************************************************/
		/* Python test                                                          */
		/************************************************************************/
// 		script::PythonScriptEngine scriptEngine(pCore);
// 		scriptEngine.exec("test = 12.5");
// 		float test = static_cast<float>(scriptEngine.eval("test"));

		/************************************************************************/
		/* Initialize entity system                                             */
		/************************************************************************/
		world::ComponentManager *pComponentMgr = new world::ComponentManager(pCore);
		pCore->addProcess(pComponentMgr);

		world::RenderBoxComponent *pRenderBoxComponent = new world::RenderBoxComponent(pCore, pComponentMgr);
		Renderer *	pScene	= pRenderBoxComponent;

		pComponentMgr->addComponent("Move", new world::MoveComponent(pCore, pComponentMgr));
		pComponentMgr->addComponent("Input", new world::InputComponent(pCore, pComponentMgr));
		pComponentMgr->addComponent("RenderBox", pRenderBoxComponent);

		for (u32 i = 0; i < 50; i++)
		{
			world::Entity ent = world::Entity::create("Test", pComponentMgr);
			ent.addComponent("Move");
			ent.addComponent("Input");
			ent.addComponent("RenderBox");

			math::Vector3f pos(
				(rand() % 1000 - 500) * 0.1f,
				(rand() % 1000 - 500) * 0.1f,
				(rand() % 1000 - 500) * 0.1f);
			world::TransformData *pTransform = ent.getData()->get<world::TransformData>("Transform");
			pTransform->position = pos;

			world::MoveData *pMove = ent.getData()->get<world::MoveData>("Move");
			pMove->velocity = pos * math::Vector3f(0.01f, 0, 0.01f);
			pMove->velocity.y += (rand() % 1000) * 0.01f;
			pMove->gravity = math::Vector3f(0, (fabs(pos.x) + fabs(pos.z)) * -0.01f, 0);
			pMove->damping = math::Vector3f(0.8f, 0.8f, 0.8f);
		}

		/************************************************************************/
		/* Initialize render process                                            */
		/************************************************************************/
		pCore->getDriver()->getContext()->bind();

		Renderer *	pRenderStart	= pCore->getDriver()->createRenderer("Start");
		Renderer *	pStartFB		= pCore->getDriver()->createRenderer("StartFB");
		Renderer *	pDrawSWF		= new render::DrawSWF(pCore);
		Renderer *	pDrawFPS		= new render::DrawFPS(pCore);
		Renderer *	pConsole		= new render::DrawConsole(pCore, console);
		Renderer *	pThreadUsage	= new render::DrawThreadUsage(pCore, threadUsage);
		Renderer *	pEndFB			= pCore->getDriver()->createRenderer("EndFB");
		Renderer *	pRenderEnd		= pCore->getDriver()->createRenderer("End");

		RenderChain *pChain = new RenderChain(pCore);
		pChain->link(pRenderStart)
			->link(pStartFB)
			->link(pScene)
			->link(pEndFB)
			->link(pDrawSWF)
			->link(pConsole)
			->link(pThreadUsage)
			->link(pDrawFPS)
			->link(pRenderEnd);

		RendererProc *pRenderProc = new RendererProc(pCore);
		pRenderProc->setDelay(0.01667); // 60 fps
		pRenderProc->setRenderer(pChain);
		pCore->addProcess(pRenderProc);

		/************************************************************************/
		/* Preload some resources                                               */
		/************************************************************************/
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
		pStartFB->as<StartFrameBuffer>()->setFrameBuffer(fb);
		pEndFB->as<EndFrameBuffer>()->setFrameBuffer(fb);

		ProgramResource programDef(pLoader);
		programDef->addShader("File::2d/shaders/bloom.frag");
		programDef->addShader("File::2d/shaders/bloom.vert");
		programDef->setLoaded(true);
		GLProgramResource program(programDef);
		program.load(ResourceLoaderBase::FLAG_ASYNC);
		pEndFB->as<EndFrameBuffer>()->setProgram(program);

#ifdef ENABLE_ASYNC_LOAD_TEST
		pCore->getLoader()->addLoader(new StreamResourceTestLoader());
		for (u32 i = 0; i < 500; i++)
		{
			char pName[32];
			sprintf(pName, "Test::testing_%d", i);

			StreamResourceTestResource test(pCore->getLoader(), pName);
			test->setLoadPriority(-250 + i);
			test.load(ResourceLoaderBase::FLAG_ASYNC);
		}
#endif

		pCore->getDriver()->getContext()->unbind();

		/************************************************************************/
		/* Run the core                                                         */
		/************************************************************************/
		pWindow->show();

		// Run process handling
		pCore->run();

#if PROCESS_OVERHEAD_TEST
		Log *log = LOG_GET_ROOT;
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

		pWindow->destroy();
		delete pWindow;

		// TODO: Clean up processes and renderers

		LOG_GET_ROOT->clearWriters();
	}
	catch (os::Exception &e)
	{
		LOG_FATAL(LOG_GET_ROOT, "Fatal exception: " << e.what());
		LOG_FATAL(LOG_GET_ROOT, "  in " << e.getFilename() << "[" << e.getLineNumber() << "]:");
		//LOG_FATAL(LOG_GET_ROOT, "  stack:" << e.getStackTrace());
	}
	catch (std::runtime_error &e)
	{
		LOG_FATAL(LOG_GET_ROOT, "Fatal exception: " << e.what());
	}

	FreeImage_DeInitialise();

	return;
}