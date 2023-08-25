#include "winapp.h"
#include "d3dapp.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>

#include <iostream>
#include <cassert>


//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
//{
//	WinApp winApp{};
//
//	winApp.CreateWindowsApp();
//
//	winApp.Run();
//}



int main()
{
	WinApp winApp{};
	winApp.CreateWindowsApp();
	

	D3DApp d3dApp{};
	d3dApp.CreateD3DApp(winApp.GetHandle());

	winApp.Show();
	

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io{ ImGui::GetIO() };
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	
	// Init only sets up ImGui to work with the window/device it doesn't create one

	ImGui_ImplWin32_Init(winApp.GetHandle());

	ImGui_ImplDX11_Init(d3dApp.GetDevice(), d3dApp.GetContext());

	bool showDemo{ true };
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	const float clear_color_with_alpha[4]{
		clear_color.x * clear_color.w,
		clear_color.y * clear_color.w,
		clear_color.z * clear_color.w,
		clear_color.w,
	};

	bool done = false;

	while (!done)
	{
		done = winApp.CheckMessages();


		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow(&showDemo);

		ImGui::Render();

		d3dApp.SetRenderTargets(clear_color_with_alpha);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());



		d3dApp.PresentSwapChain();
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	d3dApp.CleanupD3DApp();

	winApp.CleanupWinApp();


	return 0;
}
