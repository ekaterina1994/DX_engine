#include "UIManager.h"
#include "stdafx.h"
#include "Application.h"

LRESULT CALLBACK WndProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)

{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			if (MessageBox(0, L"Are you sure you want to exit?",
				L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				g_ApplicationPtr->Stop();
				DestroyWindow(hwnd);
			}
		}
		return 0;

	case WM_DESTROY: // x button on top right corner of window was pressed
		g_ApplicationPtr->Stop();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}


UIManager::UIManager()
{
	m_hwnd			= 0;
	m_isFullScreen	= false;
}

int UIManager::Init(HINSTANCE hInstance, int ShowWnd)
{
	if (m_isFullScreen)
	{
		HMONITOR hmon = MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(hmon, &mi);

		m_width = mi.rcMonitor.right - mi.rcMonitor.left;
		m_height = mi.rcMonitor.bottom - mi.rcMonitor.top;
	}
	else
	{
		m_width		= 600;
		m_height	= 500;
	}

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_windowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	m_hwnd = CreateWindowEx(NULL,
		m_windowName,
		m_windowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		m_width, m_height,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!m_hwnd)
	{
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	if (m_isFullScreen)
	{
		SetWindowLong(m_hwnd, GWL_STYLE, 0);
	}

	ShowWindow(m_hwnd, ShowWnd);
	UpdateWindow(m_hwnd);

	return OK;
}


int UIManager::getHeight()
{
	return m_height;
}

int UIManager::getWidth()
{
	return m_width;
}

HWND UIManager::getHandle()
{
	return m_hwnd;
}

bool UIManager::is_fullScreen()
{
	return m_isFullScreen;
}

int UIManager::ClearAll()
{
	return OK;
}
