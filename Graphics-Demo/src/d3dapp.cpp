#include "d3dapp.h"
#include <d3d11.h>


void D3DApp::SetRenderTargets(const FLOAT* colorRGBA)
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, colorRGBA);
}


void D3DApp::PresentSwapChain()
{
	m_swapChain->Present(1, 0); // Present with vsync
}


bool D3DApp::CreateD3DApp(HWND hwnd)
{
	// CreateDevice and Create SwapChain can be combined but then it seems you lose the ability to check if Multisampling is supported

	HRESULT hResult = D3D11CreateDevice(
		nullptr, // Display Adapter (pass nullptr to use primary)
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, // Software Driver (pass null since using hardware rendering)
		D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED,
		m_featureLevels, // An array of enums specifying which dx versions to support
		m_featureLevelLength, // the length of m_featureLevels
		D3D11_SDK_VERSION,
		&m_device,
		&m_featureLevel,
		&m_deviceContext
	);

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"D3D11CreateDevice Failed", nullptr, 0);
		return false;
	}

	if (m_featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(nullptr, L"D3D11.0 feature level not supported", nullptr, 0);
		return false;
	}

	// Check that MultiSampling is supported
	bool enableX4Msaa{ true };
	UINT x4MsaaQuality;
	m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &x4MsaaQuality);

	if (enableX4Msaa && x4MsaaQuality <= 0)
	{
		// Could just fallback to not using instead of throwing an error
		// useMsaa = false;

		MessageBox(nullptr, L"4X MultiSampling not supported", nullptr, 0);
		return false;
	}


	// Create Swap chain
	DXGI_SWAP_CHAIN_DESC& sd{ m_swapChainDesc };

	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (enableX4Msaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = x4MsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	if (!CreateSwapChain())
	{
		return false;
	}

	ID3D11Texture2D* backBuffer{};
	if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
	{
		MessageBox(nullptr, L"GetBuffer Failed", nullptr, 0);
		return false;
	}

	if (FAILED(m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView)))
	{
		MessageBox(nullptr, L"CreateRenderTargetView Failed", nullptr, 0);
		return false;
	}

	backBuffer->Release();

	return true;
}


void D3DApp::CleanupD3DApp()
{
	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

}


bool D3DApp::CreateSwapChain()
{
	if (m_swapChain)
	{
		MessageBox(nullptr, L"SwapChain Already Exists", nullptr, 0);
		return false;
	}

	// Get the Factory that was used to create the device through COM Query
	IDXGIDevice* dxgiDevice{ nullptr };
	CheckHResult(m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter{ nullptr };
	CheckHResult(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory{ nullptr };
	CheckHResult(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	// Create the new swap chain
	HRESULT hr = dxgiFactory->CreateSwapChain(m_device, &m_swapChainDesc, &m_swapChain);

	// Release references to COM objects
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"CreateSwapChain Failed", nullptr, 0);
		return false;
	}

	return true;
}


void D3DApp::CheckHResult(HRESULT hResult)
{
	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"HRESULT FAILED", nullptr, 0);
	}
}
