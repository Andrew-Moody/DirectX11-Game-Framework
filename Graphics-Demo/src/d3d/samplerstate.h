#pragma once
#include "ibindable.h"
#include "iserializable.h"

#include <wrl/client.h>

struct ID3D11SamplerState;

namespace d3d
{
	class D3DApp;
	
	class SamplerState : public ISerializable, public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element) override;

		SamplerState(D3DApp& app);

		SamplerState() = default;

	private:

		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;
	};
}
