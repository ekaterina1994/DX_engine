#pragma once
#include <Windows.h>

class UIManager
{
public:
	UIManager();
	int Init(HINSTANCE hInstance, int ShowWnd);
private:
	HWND m_hwnd;
	bool m_isFullScreen;
	int m_width;
	int m_height;

	LPCTSTR m_windowName  = L"MyApplication";
	LPCTSTR m_windowTitle = L"My Application";
};

