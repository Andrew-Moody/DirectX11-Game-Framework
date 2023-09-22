#include "vertexbuffer.h"
#include "vertex.h"

#include "d3dapp.h"
#include "d3dutil.h"

#include <sstream>

namespace d3d
{
	void VertexBuffer::bind(D3DApp& app)
	{
		DB_LOG("Binding VertexBuffer");

		constexpr UINT stride{ sizeof(Vertex) };
		constexpr UINT offset{ 0 };

		app.getContext().IASetVertexBuffers(m_slot, 1u, m_buffer.GetAddressOf(), &stride, &offset);
	}


	VertexBuffer::VertexBuffer(D3DApp& app, const std::vector<Vertex>& vertices, UINT slot)
		: m_vertexCount{ vertices.size() }, m_byteWidth{ vertices.size() * sizeof(Vertex) }, m_slot{ slot }
	{
		DB_LOG("Creating VertexBuffer, VertexCount: " << m_vertexCount << ", ByteWidth: " << m_byteWidth << ", Stride: " << sizeof(Vertex) << '\n');

		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth = m_byteWidth;
		bufferDesc.StructureByteStride = sizeof(Vertex);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA subResourceData{};
		subResourceData.pSysMem = vertices.data();

		HR(app.getDevice().CreateBuffer(&bufferDesc, &subResourceData, &m_buffer));

		DB_ASSERT(m_buffer.Get());
	}
}
