#pragma once
#include "ibindable.h"

#include <wrl/client.h>

struct ID3D11SamplerState;

namespace d3d
{
	class D3DApp;
	
	class SamplerState : public IBindable
	{
	public:

		void bind(D3DApp& app);

		SamplerState(D3DApp& app);

		SamplerState() = default;

	private:

		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;
	};
}
