#include "vertexshader.h"
#include "d3dapp.h"
#include "d3dutil.h"
#include "bytecode.h"

#include <tinyxml2.h>

#include <vector>

namespace d3d
{
	void VertexShader::bind(D3DApp& app)
	{
		DB_LOG("Binding VertexShader");

		app.getContext().VSSetShader(m_vertexShader.Get(), nullptr, 0u);
	}


	void VertexShader::deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element)
	{
		DB_ASSERT(strcmp(element->Name(), "VertexShader") == 0);

		m_byteCode = CreateShaderByteCode(element->Attribute("id"));

		HR(app.getDevice().CreateVertexShader(m_byteCode.data(), m_byteCode.size(), nullptr, &m_vertexShader));
	}


	VertexShader::VertexShader(D3DApp& app, const std::vector<uint8_t>& byteCode)
	{
		HR(app.getDevice().CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, &m_vertexShader));
	}
}
