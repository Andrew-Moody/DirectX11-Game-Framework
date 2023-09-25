#include "indexbuffer.h"
#include "d3dapp.h"
#include "d3dutil.h"

#include <d3d11.h>

#include <vector>

namespace d3d
{
	void IndexBuffer::bind(D3DApp& app)
	{
		DB_LOG("Binding IndexBuffer");

		app.getContext().IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}


	IndexBuffer::IndexBuffer(D3DApp& app, const std::vector<UINT16>& indices)
		: m_indexCount{ indices.size() }, m_byteWidth{ indices.size() * sizeof(UINT16) }
	{
		DB_LOG("Creating IndexBuffer, IndexCount: " << m_indexCount << ", ByteWidth: " << m_byteWidth << '\n');

		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth = static_cast<UINT>(m_byteWidth);
		bufferDesc.StructureByteStride = 0u;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA subResourceData{};
		subResourceData.pSysMem = indices.data();

		HR(app.getDevice().CreateBuffer(&bufferDesc, &subResourceData, &m_buffer));

		DB_ASSERT(m_buffer.Get());
	}
}
