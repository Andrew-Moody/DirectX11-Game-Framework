#include "indexbuffer.h"

#include "d3dutil.h"

namespace d3d
{
	void IndexBuffer::bind(ID3D11DeviceContext& context)
	{
		DB_LOG("Binding IndexBuffer");

		context.IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}


	IndexBuffer::IndexBuffer(ID3D11Device& device, const UINT16* idxArray, UINT byteWidth)
	{
		DB_LOG("Creating IndexBuffer, ByteWidth: " << byteWidth << '\n');

		DB_ASSERT(idxArray);

		m_bufferDesc.ByteWidth = byteWidth;
		m_bufferDesc.StructureByteStride = 0u;
		m_bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		m_bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_bufferDesc.CPUAccessFlags = 0u;
		m_bufferDesc.MiscFlags = 0u;

		m_subresourceData.pSysMem = idxArray;

		HR(device.CreateBuffer(&m_bufferDesc, &m_subresourceData, &m_buffer));

		DB_ASSERT(m_buffer.Get());
	}
}
