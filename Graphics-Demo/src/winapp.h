#pragma once

#include <windows.h>


class WinApp
{
public:

	bool CreateWindowsApp();

	void Show();

	bool CheckMessages();

	void CleanupWinApp();

	HWND GetHandle() { return m_mainWindow; };

	int GetWParam() { return static_cast<int>(m_msg.wParam); };

private:

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND m_mainWindow{ nullptr };

	WNDCLASSEXW m_wndClass{};

	LPCWSTR m_className = L"BasicWndClass";

	int m_nShowCmd{ -1 };

	MSG m_msg{};
};
