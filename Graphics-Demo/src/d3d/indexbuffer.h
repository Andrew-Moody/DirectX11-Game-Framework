#pragma once
#include "ibindable.h"

#include <wrl/client.h>

#include <vector>

struct ID3D11Buffer;

namespace d3d
{
	class D3DApp;

	class IndexBuffer : public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		size_t getIndexCount() const { return m_indexCount; }

		IndexBuffer(D3DApp& app, const std::vector<UINT16>& indices);

		IndexBuffer() = default;

		~IndexBuffer() override = default;

	private:

		size_t m_indexCount{};
		size_t m_byteWidth{};

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	};
}
