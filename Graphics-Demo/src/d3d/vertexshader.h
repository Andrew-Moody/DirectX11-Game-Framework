#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "ibindable.h"

#include <vector>

namespace d3d
{
	class VertexShader : public IBindable
	{
	public:

		void bind(ID3D11DeviceContext& context) override;

		VertexShader(ID3D11Device& device, const wchar_t* filepath);

		VertexShader() = default;

		const std::vector<char>& GetByteCode() const noexcept { return m_bytecode; }

	private:

		std::vector<char> m_bytecode{};
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader{};
	};
}