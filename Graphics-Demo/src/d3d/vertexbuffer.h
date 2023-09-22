#pragma once
#include "ibindable.h"

//#include <d3d11.h>
#include <wrl/client.h>

#include <vector>

struct ID3D11Buffer;

namespace d3d
{
	class D3DApp;
	struct Vertex;
	
	class VertexBuffer : public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		size_t getVertexCount() const { return m_vertexCount; }

		VertexBuffer(D3DApp&, const std::vector<Vertex>& vertices, UINT slot);

		VertexBuffer() = default;

		~VertexBuffer() override = default;

	private:

		const UINT m_slot{};

		size_t m_vertexCount{};

		size_t m_byteWidth{};

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	};
}
