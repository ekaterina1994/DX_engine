#pragma once
#include <map>

#include "windows.h"

#include "UIManager.h"
#include "RenderingManager.h"
#include "ResourceManager.h"

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
	void Render(); // if Render fails, then everything is fucked up and you can't do anything. Don't check every Render() call. Better check stuff inside Render().
	bool IsRunning(); // use C++, not C with classes. If the method's name starts with "Is" then it should return a boolean value in 99.9% cases.
	int Stop();
	int ClearAll();

private:
	bool m_isRunning;

	int InitManagers(HINSTANCE hInstance, int ShowWnd);
	int InitScene();

	UIManager*			m_uiManager;
	RenderingManager*	m_renderingManager;
	ResourceManager*	m_resourceManager;

};