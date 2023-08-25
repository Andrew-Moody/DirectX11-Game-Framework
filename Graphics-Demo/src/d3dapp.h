#pragma once

#include <d3d11.h>

class D3DApp
{
public:

	void SetRenderTargets(const FLOAT* colorRGBA);

	void PresentSwapChain();

	bool CreateD3DApp(HWND hwnd);

	void CleanupD3DApp();

	ID3D11Device* GetDevice() { return m_device; };
	ID3D11DeviceContext* GetContext() { return m_deviceContext; };

private:

	bool CreateSwapChain();
	void CheckHResult(HRESULT hResult);

	ID3D11Device* m_device{ nullptr };
	ID3D11DeviceContext* m_deviceContext{ nullptr };
	

	D3D_FEATURE_LEVEL m_featureLevel{}; // This will be set the first supported feature level after the call to create device
	static constexpr int m_featureLevelLength{ 1 };
	static constexpr D3D_FEATURE_LEVEL m_featureLevels[m_featureLevelLength]{ D3D_FEATURE_LEVEL_11_0 }; // This will only support dx11

	// SwapChain
	DXGI_SWAP_CHAIN_DESC m_swapChainDesc{};
	IDXGISwapChain* m_swapChain{ nullptr };

	ID3D11RenderTargetView* m_renderTargetView{};

	ID3D11DepthStencilView* m_depthStencilView{};
};
