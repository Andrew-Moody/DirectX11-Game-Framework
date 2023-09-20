#pragma once
#include "ibindable.h"

#include <d3d11.h>
#include <wrl/client.h>

struct ID3D11Buffer;

namespace d3d
{
	class D3DApp;
	
	class VertexBuffer : public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		VertexBuffer(D3DApp& app, const void* vxArray, UINT byteWidth, UINT stride);

		VertexBuffer() = default;

		~VertexBuffer() override = default;

	private:

		D3D11_BUFFER_DESC m_bufferDesc{};
		D3D11_SUBRESOURCE_DATA m_subresourceData{};
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;

		const UINT m_strides{};

		const UINT m_offsets{};
	};
}
