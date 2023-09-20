#include "vertexbuffer.h"
#include "d3dapp.h"
#include "d3dutil.h"

#include <sstream>

namespace d3d
{
	void VertexBuffer::bind(D3DApp& app)
	{
		DB_LOG("Binding VertexBuffer");

		app.getContext().IASetVertexBuffers(0u, 1u, m_buffer.GetAddressOf(), &m_strides, &m_offsets);
	}


	VertexBuffer::VertexBuffer(D3DApp& app, const void* vxArray, UINT byteWidth, UINT stride)
		: m_strides{stride}
	{
		DB_LOG("Creating VertexBuffer, ByteWidth: " << byteWidth << ", Stride: " << stride << '\n');

		DB_ASSERT(vxArray);

		m_bufferDesc.ByteWidth = byteWidth;
		m_bufferDesc.StructureByteStride = stride;
		m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_bufferDesc.CPUAccessFlags = 0u;
		m_bufferDesc.MiscFlags = 0u;

		m_subresourceData.pSysMem = vxArray;

		// Note existing buffer gets released
		HR(app.getDevice().CreateBuffer(&m_bufferDesc, &m_subresourceData, &m_buffer));

		DB_ASSERT(m_buffer.Get());
	}
}
