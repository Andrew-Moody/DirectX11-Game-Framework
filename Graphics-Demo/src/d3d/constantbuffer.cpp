#include "constantbuffer.h"
#include "d3dapp.h"

#include <d3d11.h>
#include "d3dutil.h"

namespace d3d
{
	template<typename T>
	void ConstantBuffer<T>::bind(D3DApp& app)
	{
		DB_LOG("Binding ConstantBuffer to slot " << m_startSlot);

		app.getContext().VSSetConstantBuffers(m_startSlot, 1u, m_buffer.GetAddressOf());
	}


	template<typename T>
	void ConstantBuffer<T>::setData(D3DApp& app, const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource{};

		HR(app.getContext().Map(m_buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedResource));

		memcpy(mappedResource.pData, &data, sizeof(T));

		app.getContext().Unmap(m_buffer.Get(), 0u);
	}


	template<typename T>
	ConstantBuffer<T>::ConstantBuffer(D3DApp& app, UINT startSlot, const T& data)
		: m_startSlot{ startSlot }
	{
		DB_LOG("Creating ContextBuffer, ByteWidth: " << sizeof(T) << '\n');

		D3D11_BUFFER_DESC m_bufferDesc{};
		
		m_bufferDesc.ByteWidth = sizeof(T);
		m_bufferDesc.StructureByteStride = 0u;
		m_bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		m_bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_bufferDesc.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA m_subresourceData{};

		m_subresourceData.pSysMem = &data;

		HR(app.getDevice().CreateBuffer(&m_bufferDesc, &m_subresourceData, &m_buffer));

		DB_ASSERT(m_buffer.Get());
	}


	template class ConstantBuffer<DirectX::XMMATRIX>;
}
