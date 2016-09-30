#pragma once

#include "windows.h"

#include "UIManager.h"
#include "RenderingManager.h"
#include "ResourceManager.h"

class Application
{
	friend class UIManager;
	friend class RenderingManager;
	friend class ResourceManager;

public:
	Application();
	int Init(HINSTANCE hInstance, int ShowWnd);
	int Run();
	int ClearAll();

	bool m_isRunning;

private:
	UIManager*			app_uiManager;
	RenderingManager*	app_renderingManager;
	ResourceManager*	app_resourceManager;
};