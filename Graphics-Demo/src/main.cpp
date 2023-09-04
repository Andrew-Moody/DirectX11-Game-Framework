#include "d3dapp.h"
#include "d3d/d3dutil.h"

#include <imgui.h>

#include <d3d11.h>

#include <iostream>
#include <cassert>


void RunWithImGui()
{
	d3d::D3DApp d3dApp{};
	HR(d3dApp.CreateD3DApp());


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io{ ImGui::GetIO() };
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Sets up ImGui to work with the window/device
	d3dApp.ImGuiInit();

	bool showDemo{ false };
	const ImVec4 clear_color{ 0.45f, 0.55f, 0.60f, 0.10f };
	const float clear_color_with_alpha[4]{
		clear_color.x * clear_color.w,
		clear_color.y * clear_color.w,
		clear_color.z * clear_color.w,
		clear_color.w,
	};

	bool done = false;

	while (!done)
	{
		done = d3dApp.CheckMessages();

		d3dApp.ImGuiNewFrame();

		ImGui::NewFrame();

		ImGui::ShowDemoWindow(&showDemo);

		ImGui::Render();

		d3dApp.SetRenderTargets(&clear_color_with_alpha[0]);

		d3dApp.Draw();

		d3dApp.ImGuiRenderDrawData();

		d3dApp.PresentSwapChain();
	}

	d3dApp.ImGuiShutdown();

	ImGui::DestroyContext();
}


void RunWithoutImGui()
{
	d3d::D3DApp d3dApp{};
	HR(d3dApp.CreateD3DApp());

	const float clear_color[4]{ 0.45f, 0.55f, 0.60f, 1.0f };

	bool done = false;

	while (!done)
	{
		done = d3dApp.CheckMessages();

		d3dApp.SetRenderTargets(&clear_color[0]);

		d3dApp.Draw();

		d3dApp.PresentSwapChain();
	}
}




int main()
{
	RunWithoutImGui();

	d3dutil::ReportLiveObjects();
	
	return 0;
}