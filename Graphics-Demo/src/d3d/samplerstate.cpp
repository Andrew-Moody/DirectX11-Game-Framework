#include "samplerstate.h"

#include "d3dapp.h"
#include "d3dutil.h"

namespace d3d
{
	void SamplerState::bind(D3DApp& app)
	{
		DB_LOG("Binding SamplerState")

		app.getContext().PSSetSamplers(0u, 1u, m_sampler.GetAddressOf());
	}


	SamplerState::SamplerState(D3DApp& app)
	{
		D3D11_SAMPLER_DESC samplerDesc{};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		HR(app.getDevice().CreateSamplerState(&samplerDesc, &m_sampler));

	}
}
