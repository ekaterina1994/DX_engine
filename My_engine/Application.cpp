#include "Application.h"
#include "stdafx.h"
#include <stdio.h>

Application::Application()
{
	app_uiManager			= nullptr;
	app_renderingManager	= nullptr;
	app_resourceManager		= nullptr;
	m_isRunning = true;
}

int Application::Init(HINSTANCE hInstance, int ShowWnd)
{
	int initialization_status = 0;
	if (!app_uiManager)
	{
		app_uiManager = new UIManager();
		if (app_uiManager->Init(hInstance, ShowWnd))
		{
			printf("UIManager initialization failed!");
			initialization_status += EXIT_FAILURE;
		}
	}

	if (!app_renderingManager)
	{
		app_renderingManager = new RenderingManager();
		if (app_renderingManager->Init())
		{
			printf("RenderingManager initialization failed!");
			initialization_status += EXIT_FAILURE;
		}
	}

	if (!app_resourceManager)
	{
		app_resourceManager = new ResourceManager();
		app_resourceManager->Init();
	}

	return initialization_status;
}

int Application::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); 
	OutputDebugString(L"I have started!\n");
	while (m_isRunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			// run game code
			//Update(); // update the game logic
			//Render(); // execute the command queue (rendering the scene is the result of the gpu executing the command lists)
		}
	}
	return EXIT_SUCCESS;
}

int Application::ClearAll()
{
	delete app_uiManager;
	delete app_renderingManager;
	delete app_resourceManager;

	return EXIT_SUCCESS;
}