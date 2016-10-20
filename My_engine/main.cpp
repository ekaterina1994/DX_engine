#include "stdafx.h"
#include "Application.h"

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	Application* MainApp = new Application();
	if (MainApp->Init(hInstance, nShowCmd) == EXIT_FAILURE)
	{
		delete MainApp;
		return EXIT_FAILURE;
	}
	if (MainApp->Run() == EXIT_FAILURE)
	{
		delete MainApp;
		return EXIT_FAILURE;
	}
	if (MainApp->ClearAll() == EXIT_FAILURE)
	{
		delete MainApp;
		return EXIT_FAILURE;
	}

	delete MainApp;
	return 0;
}