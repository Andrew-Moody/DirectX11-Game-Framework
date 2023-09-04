#pragma once

#include "winapp.h"

#include <d3d11.h>
#include <vector>
#include <wrl/client.h>
#include <cmath>

#include <DirectxMath.h>

namespace d3d
{
	using Microsoft::WRL::ComPtr;
	using HRESULT = long;

	
	struct Vertex
	{
		float x;
		float y;
		float z;

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};


	class D3DApp
	{
	public:

		void SetRenderTargets(const FLOAT* colorRGBA);

		void PresentSwapChain();

		HRESULT CreateD3DApp();

		void ImGuiInit();

		void ImGuiNewFrame();

		void ImGuiRenderDrawData();

		void ImGuiShutdown();

		void Draw();

		bool CheckMessages();

		D3DApp() = default;
		virtual ~D3DApp();
		D3DApp(const D3DApp& arg) = delete;
		D3DApp& operator=(const D3DApp& arg) = delete;
		D3DApp(D3DApp&& arg) = delete;
		D3DApp& operator=(D3DApp&& arg) = delete;

	private:

		HRESULT CheckFeatures();

		HRESULT CreateSwapChain();

		HRESULT CreateDepthStencilBuffer();

		void SetViewPort();

		void CreateGeometry();

		void CreateShaders();

		std::vector<char> CreateShaderByteCode(const wchar_t* path);

	private:

		WinApp m_window{};

		const int m_width{ 800 };
		const int m_height{ 800 };
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


		// Shaders
		D3D11_INPUT_ELEMENT_DESC m_vertexElementDesc[2]{};
		ComPtr<ID3D11InputLayout> m_vertexInputLayout{};
		ComPtr<ID3D11VertexShader> m_vertexShader{};
		ComPtr<ID3D11PixelShader> m_pixelShader{};

		D3D11_INPUT_ELEMENT_DESC m_ElementDesc{};

		// Geometry
		D3D11_BUFFER_DESC m_verBufferDesc{};
		D3D11_SUBRESOURCE_DATA m_verSubresourceData{};
		ComPtr<ID3D11Buffer> m_vertexBuffer;

		D3D11_BUFFER_DESC m_idxBufferDesc{};
		D3D11_SUBRESOURCE_DATA m_idxSubresourceData{};
		ComPtr<ID3D11Buffer> m_indexBuffer;

		D3D11_BUFFER_DESC m_constantBufferDesc{};
		D3D11_SUBRESOURCE_DATA m_constantSubResData{};
		ComPtr<ID3D11Buffer> m_constantBuffer{};

		Vertex m_vertices[8]
		{
			/*{0.0f, 0.0f, 0.0f},
			{0.5f, 0.0f, 0.0f},
			{0.0f, 0.5f, 0.0f},
			{0.5f, 0.5f, 0.0f}*/

			/*{-0.5f, -0.433f, 0.0f,		255u, 0u, 0u, 128},
			{0.0f, 0.433f, 0.0f,		0u, 255u, 0u, 128},
			{0.5f, -0.433f, 0.0f,		0u, 0u, 255u, 128}*/

			{-0.5f,	-0.5f,	-0.5f,	255u, 0u,	0u,   0u},
			{-0.5f,	0.5f,	-0.5f,	0u,   255u, 0u,   0u},
			{0.5f,	0.5f,	-0.5f,	0u,	  0u,	255u, 0u},
			{0.5f,	-0.5f,	-0.5f,	255u, 0u,	255u, 0u},

			{-0.5f,	-0.5f,	0.5f,	255u, 0u,	0u,   0u},
			{-0.5f,	0.5f,	0.5f,	0u,   255u, 0u,   0u},
			{0.5f,	0.5f,	0.5f,	0u,	  0u,	255u, 0u},
			{0.5f,	-0.5f,	0.5f,	255u, 0u,	255u, 0u}
		};

		const uint16_t m_indices[36]
		{
			0, 1, 2,	0, 2, 3,
			1, 5, 6,	1, 6, 2,
			0, 4, 5,	0, 5, 1,
			0, 3, 7,    0, 7, 4,
			2, 6, 7,	2, 7, 3,
			4, 5, 6,	4, 6, 7
		};

		const UINT m_strides = sizeof(Vertex);

		const UINT m_offsets = 0;

		const float radToDeg{ 0.017453f };

		float m_roll{ 10.0f * radToDeg };
		float m_pitch{ 20.0f * radToDeg };
		float m_yaw{ -10.0f * radToDeg };

		DirectX::XMMATRIX m_transform{};
	};


	
}
