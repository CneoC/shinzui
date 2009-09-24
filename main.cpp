#include <cstdio>


#include "core/Core.h"
//#include "util/LogTestProc.h"
#include "os/GLWindow.h"
#include "render/2d/Console.h"
#include "render/2d/DrawFPS.h"
#include "render/scene/Scene.h"
#include "render/StartRenderer.h"
#include "render/EndRenderer.h"
#include "render/StartFrameBuffer.h"
#include "render/EndFrameBuffer.h"

#include "resources/FontResource.h"

void main(const char *argc, int argv)
{
	{
		FontData *pData = new FontData();
		FontResource res1(pData);
		FontResource res2;
		{
			FontResource res3(res1);
			{
				FontResource res4(res1);
			}
		}

	}


	Core *pCore = new Core(0);
	
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
	GLWindow		*pWindow		= new GLWindow(pCore);
	pWindow->setTitle("Engine");
	pWindow->setSize(Vector2i(1024, 768));
	pWindow->create();

	StartRenderer		*pRenderStart	= new StartRenderer(pCore);
	StartFrameBuffer	*pStartFBO		= new StartFrameBuffer(pCore);
	Scene				*pScene			= new Scene(pCore);
	EndFrameBuffer		*pEndFBO		= new EndFrameBuffer(pCore);
	Console				*pConsole		= new Console(pCore);
	DrawFPS				*pFps			= new DrawFPS(pCore);
	EndRenderer			*pRenderEnd		= new EndRenderer(pCore);

	// Link the render processes
	pRenderStart
		->link(pStartFBO)
		->link(pScene)
		->link(pEndFBO)
		->link(pConsole)
		->link(pFps)
		->link(pRenderEnd)
		->link(pWindow)
		->link(pRenderStart);

	pCore->addProcess(pRenderStart);

	// Run process handling
	pCore->run();

	// Clean up
	delete pWindow;
	delete pRenderEnd;
	delete pFps;
	delete pConsole;
	delete pEndFBO;
	delete pScene;
	delete pStartFBO;
	delete pRenderStart;

	delete pCore;

	return;
}