#pragma once
#include "ibindable.h"
#include "iserializable.h"

#include <wrl/client.h>

#include <vector>

struct ID3D11VertexShader;

namespace d3d
{
	class D3DApp;
	class InputLayout;

	class VertexShader : public ISerializable, public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element) override;

		VertexShader(D3DApp& app, const std::vector<uint8_t>& byteCode);

		const std::vector<uint8_t>& getByteCode() const { return m_byteCode; }

		VertexShader() = default;

	private:

		std::vector<uint8_t> m_byteCode{};

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader{};
	};
}