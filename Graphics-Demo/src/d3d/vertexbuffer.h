#pragma once

#include "ibindable.h"
#include <wrl.h>

namespace d3d
{
	using Microsoft::WRL::ComPtr;

	class VertexBuffer : public IBindable
	{
	public:

		void bind(ID3D11DeviceContext& context) override;

		VertexBuffer(ID3D11Device& device, const void* vxArray, UINT byteWidth, UINT stride);

		VertexBuffer() = default;

		~VertexBuffer() override = default;

	private:

		D3D11_BUFFER_DESC m_bufferDesc{};
		D3D11_SUBRESOURCE_DATA m_subresourceData{};
		ComPtr<ID3D11Buffer> m_buffer;

		const UINT m_strides{};

		const UINT m_offsets{};
	};
}
