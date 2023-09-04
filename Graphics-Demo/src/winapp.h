#pragma once

#include <windows.h>


class WinApp
{
public:

	HRESULT CreateWindowsApp(int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);

	void Show();

	bool CheckMessages();

	HWND GetHandle() noexcept { return m_mainWindow; }

	WPARAM GetWParam() noexcept { return m_msg.wParam; }

	int GetWidth() const noexcept { return m_rect.right - m_rect.left; }
	int GetHeight() const noexcept { return m_rect.bottom - m_rect.top; }
	float GetAspectRatio() const noexcept { return static_cast<float>(GetWidth()) / GetHeight(); }
	
	WinApp() = default;

	~WinApp();

	WinApp(const WinApp& arg) = delete;
	WinApp& operator=(const WinApp& arg) = delete;
	WinApp(WinApp&& arg) = delete;
	WinApp& operator=(WinApp&& arg) = delete;

private:

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HINSTANCE m_hinstance{};

	HWND m_mainWindow{};

	WNDCLASSEXW m_wndClass{};

	LPCWSTR m_className = L"BasicWndClass";

	int m_nShowCmd{ -1 };
	LPWSTR m_lpCmdLine{};

	MSG m_msg{};

	RECT m_rect{};
};
