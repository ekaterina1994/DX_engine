#include "stdafx.h"

Application* MainApp = nullptr;

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	MainApp = new Application();

	if (MainApp->Init(hInstance, nShowCmd) != EXIT_SUCCESS)
	{
		delete MainApp;
		return EXIT_FAILURE;
	}
	if (MainApp->Run() != EXIT_SUCCESS)
	{
		delete MainApp;
		return EXIT_FAILURE;
	}
	if (MainApp->ClearAll() != EXIT_SUCCESS)
	{
		delete MainApp;
		return EXIT_FAILURE;
	}

	delete MainApp;
	return 0;
}