#include "d3dapp.h"
#include "d3d/d3dutil.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <d3d11sdklayers.h>
#include <DirectXMath.h>

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <vector>
#include <string>

#include <sstream>
#include <utility>


namespace d3d
{
	D3DApp::D3DApp(int width, int height)
		: m_width{width}, m_height{height},
		m_success{CreateD3DApp()},
		m_cube{*m_device.Get(), m_aspect}
	{
		
	}


	void D3DApp::SetRenderTargets(const FLOAT* colorRGBA)
	{
		// Be aware that the address of operator returns an interface pointer but it also calls release
		// This is nice for cases where you are creating a new resource and want any existing resource to be released (see swapchain)
		// use GetAddressOf() if you don't want to release as in this case
		m_context->OMSetRenderTargets(1u, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

		m_context->ClearRenderTargetView(m_renderTargetView.Get(), colorRGBA);

		m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	}


	void D3DApp::PresentSwapChain()
	{
		m_swapChain->Present(1u, 0u);
		//HR(m_swapChain->Present(1u, 0u)); // Present with vsync
	}

	// Inititialize ImGui implmentation for dx11/win32
	void D3DApp::ImGuiInit()
	{
		ImGui_ImplWin32_Init(m_window.GetHandle());

		ImGui_ImplDX11_Init(m_device.Get(), m_context.Get());
	}

	void D3DApp::ImGuiRenderDrawData()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}


	void D3DApp::ImGuiNewFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
	}


	void D3DApp::ImGuiShutdown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}

	void D3DApp::Draw()
	{
		m_cube.draw(*m_context.Get());
	}


	bool D3DApp::CheckMessages()
	{
		return m_window.CheckMessages();
	}
	
	
	HRESULT D3DApp::CreateD3DApp()
	{
		HR_RETURN(m_window.CreateWindowsApp(m_width, m_height));
		m_window.Show();

		// CreateDevice and Create SwapChain can be combined but then it seems you lose the ability to check if Multisampling is supported
		HR_RETURN(D3D11CreateDevice(
			nullptr, // Display Adapter (pass nullptr to use primary)
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr, // Software Driver (pass null since using hardware rendering)
			D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED,
			&m_featureLevels[0], // An array of enums specifying which dx versions to support
			m_featureLevelLength, // the length of m_featureLevels
			D3D11_SDK_VERSION,
			&m_device,
			&m_featureLevel,
			&m_context
		));

		const char deviceName[] = "D3DApp Device";
		m_device->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(deviceName) - 1, &deviceName[0]);

		const char contextName[] = "D3DApp Context";
		m_context->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(contextName) - 1, &contextName[0]);

		HR_RETURN(CheckFeatures());

		HR_RETURN(CreateSwapChain());
		
		ComPtr<ID3D11Texture2D> backBuffer{};
		HR_RETURN(m_swapChain->GetBuffer(0u, __uuidof(ID3D11Texture2D), &backBuffer));

		HR_RETURN(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView));

		HR_RETURN(CreateDepthStencilBuffer());

		SetViewPort();

		return S_OK;
	}


	HRESULT D3DApp::CheckFeatures()
	{
		if (m_featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			MessageBox(nullptr, L"D3D11.0 feature level not supported", nullptr, 0u);
			return E_ABORT;
		}

		// Check that MultiSampling is supported
		UINT x4MsaaQuality;
		m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &x4MsaaQuality);

		if (m_enable4XMSAA && x4MsaaQuality <= 0)
		{
			MessageBox(nullptr, L"4X MultiSampling not supported", nullptr, 0u);
			m_enable4XMSAA = false;
		}

		if (m_enable4XMSAA)
		{
			m_sampleDesc.Count = 4u;
			m_sampleDesc.Quality = x4MsaaQuality - 1u;
		}
		else
		{
			m_sampleDesc.Count = 1u;
			m_sampleDesc.Quality = 0u;
		}

		return S_OK;
	}


	HRESULT D3DApp::CreateSwapChain()
	{
		if (m_swapChain)
		{
			MessageBoxW(nullptr, L"SwapChain Already Exists", nullptr, 0u);
			return E_FAIL;
		}

		DXGI_SWAP_CHAIN_DESC& sd{ m_swapChainDesc };

		sd.BufferDesc.Width = m_window.GetWidth();
		sd.BufferDesc.Height = m_window.GetHeight();
		sd.BufferDesc.RefreshRate.Numerator = 60u;
		sd.BufferDesc.RefreshRate.Denominator = 1u;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc = m_sampleDesc;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1u;
		sd.OutputWindow = m_window.GetHandle();
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // Fine for now but preffer DXGI_SWAP_EFFECT_FLIP_DISCARD (requires BufferCount of at least 2 though);
		sd.Flags = 0u;

		// Get the Factory that was used to create the device through COM Query
		ComPtr<IDXGIDevice> dxgiDevice{};
		
		HR_RETURN(m_device->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice));

		ComPtr<IDXGIAdapter> dxgiAdapter{ nullptr };
		HR_RETURN(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), &dxgiAdapter));

		ComPtr<IDXGIFactory> dxgiFactory{ nullptr };
		HR_RETURN(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory));

		// Create the new swap chain
		// Note that the interface pointer for swap chain is accessed with & rather than GetAddressOf()
		// This means if a swap chain already existed it would get released automatically
		HR_RETURN(dxgiFactory->CreateSwapChain(m_device.Get(), &m_swapChainDesc, &m_swapChain));

		return S_OK;
	}


	HRESULT D3DApp::CreateDepthStencilBuffer()
	{
		D3D11_TEXTURE2D_DESC depthStencilDesc{};
		depthStencilDesc.Width = m_window.GetWidth();
		depthStencilDesc.Height = m_window.GetHeight();
		depthStencilDesc.MipLevels = 1u;
		depthStencilDesc.ArraySize = 1u;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc = m_sampleDesc;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0u;
		depthStencilDesc.MiscFlags = 0u;

		HR_RETURN(m_device->CreateTexture2D(&depthStencilDesc, nullptr, &m_depthStencilTexture));

		HR_RETURN(m_device->CreateDepthStencilView(m_depthStencilTexture.Get(), nullptr, &m_depthStencilView));

		return S_OK;
	}


	void D3DApp::SetViewPort()
	{
		m_viewPort.TopLeftX = 0.0f;
		m_viewPort.TopLeftY = 0.0f;
		m_viewPort.Width = static_cast<float>(m_window.GetWidth());
		m_viewPort.Height = static_cast<float>(m_window.GetHeight());
		m_viewPort.MinDepth = 0.0f;
		m_viewPort.MaxDepth = 1.0f;

		m_context->RSSetViewports(1, &m_viewPort);
	}


	D3DApp::~D3DApp()
	{
		if (m_context)
		{
			try
			{
				m_context->ClearState();

				m_context->Flush();

				DB_LOG("ClearState and Flush\n");
			}
			catch (...)
			{
				DB_LOG("~D3DApp threw an exception\n");
			}
		}
	}
}
