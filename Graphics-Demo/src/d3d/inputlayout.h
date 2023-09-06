#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "ibindable.h"

#include <vector>

namespace d3d
{
	using Microsoft::WRL::ComPtr;

	class InputLayout : public IBindable
	{
	public:

		void bind(ID3D11DeviceContext& context) override;

		InputLayout(ID3D11Device& device, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDescs, const std::vector<char>& bytecode);

		InputLayout() = default;

	private:

		D3D11_INPUT_ELEMENT_DESC m_inputDesc{};
		ComPtr<ID3D11InputLayout> m_inputLayout{};
	};
}
