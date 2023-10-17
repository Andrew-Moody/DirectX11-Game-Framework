#include "inputlayout.h"
#include "d3dapp.h"
#include "d3dutil.h"

#include <tinyxml2.h>

namespace d3d
{
	using std::vector;

	void InputLayout::bind(D3DApp& app)
	{
		DB_LOG("Binding InputLayout");

		app.getContext().IASetInputLayout(m_inputLayout.Get());
	}


	void InputLayout::deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element)
	{
		using namespace tinyxml2;

		DB_ASSERT(strcmp(element->Name(), "InputLayout") == 0);

		const XMLElement* vsElement = element->FirstChildElement("VertexShader");

		DB_ASSERT(vsElement);

		VertexShader* vertexShader = dynamic_cast<VertexShader*>(app.getResourceManager().getResource(vsElement->Attribute("id")));

		DB_ASSERT(vertexShader);

		std::vector<D3D11_INPUT_ELEMENT_DESC> elementDescs{
			{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT,  0u, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"TexCoord", 0u, DXGI_FORMAT_R32G32_FLOAT,  0u, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"Tangent", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT,  0u, 32u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"Weights", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT,  0u, 48u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"BoneIndices", 0u, DXGI_FORMAT_R8G8B8A8_UINT,  0u, 64u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
		};


		const std::vector<uint8_t>& byteCode = vertexShader->getByteCode();

		DB_LOG("Creating InputLayout, inputDescs size: " << elementDescs.size() << ", bytecode size: " << byteCode.size() << '\n');

		HR(app.getDevice().CreateInputLayout(elementDescs.data(), static_cast<UINT>(elementDescs.size()), byteCode.data(), byteCode.size(), &m_inputLayout));

		assert(m_inputLayout.Get());
	}


	InputLayout::InputLayout(D3DApp& app, const vector<D3D11_INPUT_ELEMENT_DESC>& inputDescs, const vector<uint8_t>& byteCode)
	{
		DB_LOG("Creating InputLayout, inputDescs size: " << inputDescs.size() << ", bytecode size: " << byteCode.size() << '\n');

		HR(app.getDevice().CreateInputLayout(inputDescs.data(), static_cast<UINT>(inputDescs.size()), byteCode.data(), byteCode.size(), &m_inputLayout));

		assert(m_inputLayout.Get());
	}
}
