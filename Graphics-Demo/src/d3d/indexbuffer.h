#pragma once
#include "ibindable.h"
#include <d3d11.h>
#include <wrl/client.h>

struct ID3D11Buffer;

namespace d3d
{
	class D3DApp;

	class IndexBuffer : public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		IndexBuffer(D3DApp& app, const UINT16* idxArray, UINT byteWidth);

		IndexBuffer() = default;

		~IndexBuffer() override = default;

	private:

		D3D11_BUFFER_DESC m_bufferDesc{};
		D3D11_SUBRESOURCE_DATA m_subresourceData{};
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	};
}
