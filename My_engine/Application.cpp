#include "Application.h"
#include "Model.h"

#include "stdafx.h"

#include <stdio.h>


Application* g_ApplicationPtr;

Application::Application()
{
	g_ApplicationPtr = this;

	m_uiManager			= nullptr;
	m_renderingManager	= nullptr;
	m_resourceManager	= nullptr;

	m_isRunning = true;
}

int Application::InitManagers(HINSTANCE hInstance, int ShowWnd)
{
	int initialization_status = EXIT_SUCCESS;
	
	if (!m_uiManager)
	{
		m_uiManager = new UIManager();
		if (m_uiManager->Init(hInstance, ShowWnd) == EXIT_FAILURE)
		{
			OutputDebugString(L"UIManager initialization failed!");
			initialization_status = EXIT_FAILURE;
		}
	}

	if (!m_renderingManager)
	{
		m_renderingManager = new RenderingManager();
		if (m_renderingManager->Init() == EXIT_FAILURE)
		{
			OutputDebugString(L"RenderingManager initialization failed!");
			initialization_status = EXIT_FAILURE;
		}
	}

	if (!m_resourceManager)
	{
		m_resourceManager = new ResourceManager();
		if (m_resourceManager->Init() == EXIT_FAILURE)
		{
			OutputDebugString(L"ResourceManager initialization failed!");
			initialization_status = EXIT_FAILURE;
		}
	}

	return initialization_status;
}

int Application::InitScene()
{
	return m_resourceManager->InitScene();
}

int Application::Init(HINSTANCE hInstance, int ShowWnd)
{
	int initialization_status = EXIT_SUCCESS;

	if (InitManagers(hInstance, ShowWnd) == EXIT_FAILURE)
	{
		OutputDebugString(L"Managers initialization failed!");
		initialization_status = EXIT_FAILURE;
	}

	if (InitScene() == EXIT_FAILURE)
	{
		OutputDebugString(L"Models initialization failed!");
		initialization_status = EXIT_FAILURE;
	}	

	return initialization_status;
}

int Application::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); 
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
			Update(); // update the game logic
			Render(); // execute the command queue (rendering the scene is the result of the gpu executing the command lists)
		}
	}
	return EXIT_SUCCESS;
}


int Application::Update()
{
	m_resourceManager->Update();
	return EXIT_SUCCESS;
}

int Application::Render()
{
	m_renderingManager->RenderFrame();
	return EXIT_SUCCESS;
}

int Application::IsRunning()
{
	return m_isRunning ? EXIT_SUCCESS : EXIT_FAILURE;
}

int Application::Stop()
{
	m_isRunning = false;
	return EXIT_SUCCESS;
}

int Application::ClearAll()
{
	if (m_resourceManager)
	{
		m_resourceManager->ClearAll();
		delete m_resourceManager;
	}

	if (m_renderingManager)
	{
		m_renderingManager->ClearAll();
		delete m_renderingManager;
	}

	if (m_uiManager)
	{
		m_uiManager->ClearAll();
		delete m_uiManager;
	}

	return EXIT_SUCCESS;
}