#include "winapp.h"
#include <windows.h>

#include <iostream>
#include <cassert>
#include <string>


HRESULT WinApp::CreateWindowsApp(int width, int height)
{
	// Traditionally these would be passed into winmain and saved as globals (hInstance, lpCmdLine, nCmdShow)
	// That can be avoided by getting them manually
	m_hinstance = GetModuleHandle(nullptr);

	m_lpCmdLine = GetCommandLineW();

	// Unfortunately this does not work correctly. From what I can tell its a visual studio quirk (that might be fixable but with side effects)
	// Seems wShowWindow is set to SW_HIDE (something about a potential malware attack vector)
	// The result is the same whether main or WinMain is used
	// 
	// STARTUPINFO startupInfo{};
	// GetStartupInfoW(&startupInfo);
	// m_nShowCmd = startupInfo.wShowWindow;

	m_nShowCmd = SW_SHOWDEFAULT; // Overriding nCmdShow since it seems to not be getting set properly 

	// Setup the window class
	m_wndClass.cbSize = sizeof(m_wndClass);
	m_wndClass.style = CS_CLASSDC;
	m_wndClass.lpfnWndProc = WndProc;
	m_wndClass.cbClsExtra = 0;
	m_wndClass.cbWndExtra = 0;
	m_wndClass.hInstance = m_hinstance;
	m_wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	m_wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	m_wndClass.lpszMenuName = nullptr;
	m_wndClass.lpszClassName = m_className;
	m_wndClass.hIconSm = nullptr;

	if (!RegisterClassExW(&m_wndClass))
	{
		MessageBox(nullptr, L"RegisterClassExW Failed", nullptr, 0);
		return E_FAIL;
	}

	m_mainWindow = CreateWindowExW(
		0l,
		m_className,
		L"WinApp",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,
		nullptr,
		m_hinstance,
		nullptr
	);

	if (!m_mainWindow)
	{
		MessageBox(nullptr, L"CreateWindow Failed", nullptr, 0);
		return E_FAIL;
	}

	GetWindowRect(m_mainWindow, &m_rect);

	return S_OK;
}


WinApp::~WinApp()
{
	DestroyWindow(m_mainWindow);
	UnregisterClassW(m_wndClass.lpszClassName, m_wndClass.hInstance);
}


void WinApp::Show()
{
	ShowWindow(m_mainWindow, m_nShowCmd);
	UpdateWindow(m_mainWindow);
}


bool WinApp::CheckMessages()
{
	bool quit = false;

	while (PeekMessageW(&m_msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);

		if (m_msg.message == WM_QUIT)
		{
			quit = true;
		}
	}

	return quit;
}


LRESULT CALLBACK WinApp::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//std::cout << "Handling msg: " << msg << '\n';

	switch (msg)
	{

	/*case WM_LBUTTONDOWN:
	{
		STARTUPINFO startupInfo{};
		GetStartupInfoW(&startupInfo);

		std::wstring messageString = L"nShowCmd: " + std::to_wstring(startupInfo.wShowWindow);

		MessageBox(nullptr, messageString.c_str(), L"Hello", MB_OK);
		return 0;
	}*/
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);
		}

		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	}

	// Let the default winproc handle any cases we don't
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
