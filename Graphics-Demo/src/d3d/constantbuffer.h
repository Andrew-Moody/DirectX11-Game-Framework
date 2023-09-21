#pragma once
#include "ibindable.h"

#include <wrl/client.h>

struct ID3D11Buffer;

namespace d3d
{	
	template<typename T>
	class ConstantBuffer : public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		void setData(D3DApp& app, const T& data);

		ConstantBuffer(D3DApp& app, UINT startSlot, const T& data);

		ConstantBuffer() = default;

		~ConstantBuffer() override = default;

	private:

		UINT m_startSlot{};

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer{};
	};
}
