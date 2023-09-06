#pragma once

#include "winapp.h"

#include <d3d11.h>
#include <vector>
#include <memory>
#include <wrl/client.h>
#include <cmath>

#include <DirectxMath.h>

#include "cube.h"

namespace d3d
{
	using Microsoft::WRL::ComPtr;
	using HRESULT = long;

	
	/*struct Vertex
	{
		float x;
		float y;
		float z;

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};*/


	class D3DApp
	{
	public:

		void SetRenderTargets(const FLOAT* colorRGBA);

		void PresentSwapChain();

		void ImGuiInit();

		void ImGuiNewFrame();

		void ImGuiRenderDrawData();

		void ImGuiShutdown();

		void Draw();

		bool CheckMessages();

		D3DApp(int width, int height);

		D3DApp() = default;
		virtual ~D3DApp();
		D3DApp(const D3DApp& arg) = delete;
		D3DApp& operator=(const D3DApp& arg) = delete;
		D3DApp(D3DApp&& arg) = delete;
		D3DApp& operator=(D3DApp&& arg) = delete;

	private:

		HRESULT CreateD3DApp();

		HRESULT CheckFeatures();

		HRESULT CreateSwapChain();

		HRESULT CreateDepthStencilBuffer();

		void SetViewPort();

	private:

		WinApp m_window{};

		const int m_width{ 800 };
		const int m_height{ 600 };
		const float m_aspect{ static_cast<float>(m_width) / m_height };

		ComPtr<ID3D11Device> m_device{};
		ComPtr<ID3D11DeviceContext> m_context{};

		D3D11_VIEWPORT m_viewPort{};

		D3D_FEATURE_LEVEL m_featureLevel{}; // This will be set to the first supported feature level after the call to create device
		static constexpr int m_featureLevelLength{ 1 };
		static constexpr D3D_FEATURE_LEVEL m_featureLevels[m_featureLevelLength]{ D3D_FEATURE_LEVEL_11_0 }; // This will only support dx11

		bool m_enable4XMSAA{ true };
		DXGI_SAMPLE_DESC m_sampleDesc{};

		// SwapChain
		DXGI_SWAP_CHAIN_DESC m_swapChainDesc{};
		ComPtr<IDXGISwapChain> m_swapChain{};

		ComPtr<ID3D11RenderTargetView> m_renderTargetView{};

		ComPtr<ID3D11Texture2D> m_depthStencilTexture{};
		ComPtr<ID3D11DepthStencilView> m_depthStencilView{};

		// Everything declared before this will be default constructed before CreateD3DApp is called
		const HRESULT m_success{};
		// Everything declared after this is free to assume the app has been fully constructed


		Cube m_cube{};		
	};
}
