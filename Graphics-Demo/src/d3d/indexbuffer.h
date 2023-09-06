#pragma once

#include "ibindable.h"
#include <wrl.h>

namespace d3d
{
	using Microsoft::WRL::ComPtr;

	class IndexBuffer : public IBindable
	{
	public:

		void bind(ID3D11DeviceContext& context) override;

		IndexBuffer(ID3D11Device& device, const UINT16* idxArray, UINT byteWidth);

		IndexBuffer() = default;

		~IndexBuffer() override = default;

	private:

		D3D11_BUFFER_DESC m_bufferDesc{};
		D3D11_SUBRESOURCE_DATA m_subresourceData{};
		ComPtr<ID3D11Buffer> m_buffer;
	};
}
