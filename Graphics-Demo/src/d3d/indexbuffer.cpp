#include "indexbuffer.h"
#include "d3dapp.h"
#include "d3dutil.h"

#include <d3d11.h>

namespace d3d
{
	void IndexBuffer::bind(D3DApp& app)
	{
		DB_LOG("Binding IndexBuffer");

		app.getContext().IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}


	IndexBuffer::IndexBuffer(D3DApp& app, const UINT16* idxArray, UINT byteWidth)
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

		HR(app.getDevice().CreateBuffer(&m_bufferDesc, &m_subresourceData, &m_buffer));

		DB_ASSERT(m_buffer.Get());
	}
}
