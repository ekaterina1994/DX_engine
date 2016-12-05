#include "stdafx.h"
#include "Application.h"

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	Application* MainApp = new Application();
	if (MainApp->Init(hInstance, nShowCmd) == FAIL)
	{
		delete MainApp;
		return FAIL;
	}
	if (MainApp->Run() == FAIL)
	{
		delete MainApp;
		return FAIL;
	}
	if (MainApp->ClearAll() == FAIL)
	{
		delete MainApp;
		return FAIL;
	}

	delete MainApp;
	return 0;
}