#include "constantbuffer.h"

#include "d3dutil.h"

namespace d3d
{
	void ConstantBuffer::bind(ID3D11DeviceContext& context)
	{
		DB_LOG("Binding ConstantBuffer");

		context.VSSetConstantBuffers(0u, 1u, m_buffer.GetAddressOf());
	}


	ConstantBuffer::ConstantBuffer(ID3D11Device& device, const void* data , UINT byteWidth)
	{
		DB_LOG("Creating ContextBuffer, ByteWidth: " << byteWidth << '\n');

		DB_ASSERT(data);

		m_bufferDesc.ByteWidth = byteWidth;
		m_bufferDesc.StructureByteStride = 0u;
		m_bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		m_bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_bufferDesc.MiscFlags = 0u;

		m_subresourceData.pSysMem = data;

		HR(device.CreateBuffer(&m_bufferDesc, &m_subresourceData, &m_buffer));

		DB_ASSERT(m_buffer.Get());
	}
}
