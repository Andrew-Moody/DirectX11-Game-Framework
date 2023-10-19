#include "d3d/d3dapp.h"
#include "d3d/d3dutil.h"
#include "frametime.h"

#include <imgui.h>

#include <d3d11.h>

#include <iostream>
#include <cassert>


void RunWithImGui(int width, int height)
{
	d3d::D3DApp d3dApp{width, height};

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


void RunWithoutImGui(int width, int height)
{
	d3d::D3DApp d3dApp{width, height};

	const float clear_color[4]{ 0.45f, 0.55f, 0.60f, 1.0f };

	FrameTime totalTime{};

	FrameTime frameTime{};

	int frames{};
	float sumFrameTime{};

	bool done = false;

	while (!done)
	{
		done = d3dApp.CheckMessages();

		frameTime.tick();

		sumFrameTime += frameTime.deltaTime();

		frames++;

		d3dApp.Update(frameTime.deltaTime());

		d3dApp.SetRenderTargets(&clear_color[0]);

		d3dApp.Draw();

		d3dApp.PresentSwapChain();
	}

	totalTime.tick();

	std::cout << "Total time single tick: " << totalTime.deltaTime() << '\n';
	std::cout << "Total time frame tick: " << sumFrameTime << '\n';
	std::cout << "Average frame time: " << sumFrameTime / frames << '\n';
}


int main()
{
	constexpr int windowWidth{ 800 };
	constexpr int windowHeight{ 600 };

	RunWithoutImGui(windowWidth, windowHeight);

	//RunWithImGui(windowWidth, windowHeight);

	d3dutil::ReportLiveObjects();
	
	return 0;
}
