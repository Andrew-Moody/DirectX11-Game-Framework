#pragma once
#include "ibindable.h"

#include <wrl/client.h>

#include <vector>

struct ID3D11VertexShader;

namespace d3d
{
	class D3DApp;

	class VertexShader : public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		VertexShader(D3DApp& app, const wchar_t* filepath);

		VertexShader() = default;

		const std::vector<char>& GetByteCode() const noexcept { return m_bytecode; }

	private:

		std::vector<char> m_bytecode{};
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader{};
	};
}