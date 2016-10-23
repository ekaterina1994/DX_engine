#pragma once
#include <map>

#include "windows.h"

#include "UIManager.h"
#include "RenderingManager.h"
#include "ResourceManager.h"

#include "Model.h"
#include "IScene.h"

extern class Application* g_ApplicationPtr;

class Application
{
	friend class UIManager;
	friend class RenderingManager;
	friend class ResourceManager;

public:
	Application();
	int Init(HINSTANCE hInstance, int ShowWnd);
	int Run();
	int Update();
	int Render();
	int IsRunning();
	int Stop();
	int ClearAll();

private:
	bool m_isRunning;

	int InitManagers(HINSTANCE hInstance, int ShowWnd);
	int InitScene();

	UIManager*			m_uiManager;
	RenderingManager*	m_renderingManager;
	ResourceManager*	m_resourceManager;

	IScene*				m_mainScene;
};