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
	int initialization_status = OK;
	
	if (!m_uiManager)
	{
		m_uiManager = new UIManager();
		if (m_uiManager->Init(hInstance, ShowWnd) == FAIL)
		{
			OutputDebugString(L"UIManager initialization failed!");
			initialization_status = FAIL;
		}
	}

	if (!m_renderingManager)
	{
		m_renderingManager = new RenderingManager();
		if (m_renderingManager->Init() == FAIL)
		{
			OutputDebugString(L"RenderingManager initialization failed!");
			initialization_status = FAIL;
		}
	}

	if (!m_resourceManager)
	{
		m_resourceManager = new ResourceManager();
		if (m_resourceManager->Init() == FAIL)
		{
			OutputDebugString(L"ResourceManager initialization failed!");
			initialization_status = FAIL;
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
	int initialization_status = OK;

	if (InitManagers(hInstance, ShowWnd) == FAIL)
	{
		OutputDebugString(L"Managers initialization failed!");
		initialization_status = FAIL;
	}

	if (InitScene() == FAIL)
	{
		OutputDebugString(L"Models initialization failed!");
		initialization_status = FAIL;
	}	

	return initialization_status;
}

int Application::Run()
{
	MSG msg = {}; // zero-initialize this way is better than ZeroMemory()

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
	return OK;
}


int Application::Update()
{
	m_resourceManager->Update();
	return OK;
}

void Application::Render()
{
	m_renderingManager->RenderFrame();
}

bool Application::IsRunning()
{
	return m_isRunning;
}

int Application::Stop()
{
	m_isRunning = false;
	return OK;
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

	return OK;
}