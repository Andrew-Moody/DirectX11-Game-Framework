#pragma once
#include "ibindable.h"

//#include <d3d11.h>
#include <wrl/client.h>

#include <vector>

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;

namespace d3d
{
	class D3DApp;

	class InputLayout : public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		InputLayout(D3DApp& app, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDescs, const std::vector<uint8_t>& bytecode);

		InputLayout() = default;

	private:

		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout{};
	};
}
