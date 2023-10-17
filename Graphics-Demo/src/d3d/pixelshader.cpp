#include "pixelshader.h"
#include "d3dapp.h"
#include "d3dutil.h"
#include "bytecode.h"

#include <tinyxml2.h>

#include <vector>


namespace d3d
{
	void PixelShader::bind(D3DApp& app)
	{
		DB_LOG("Binding PixelShader");

		app.getContext().PSSetShader(m_pixelShader.Get(), nullptr, 0u);
	}


	void PixelShader::deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element)
	{
		DB_ASSERT(strcmp(element->Name(), "PixelShader") == 0);

		m_byteCode = CreateShaderByteCode(element->Attribute("id"));

		HR(app.getDevice().CreatePixelShader(m_byteCode.data(), m_byteCode.size(), nullptr, &m_pixelShader));
	}


	PixelShader::PixelShader(D3DApp& app, const std::vector<uint8_t>& byteCode)
	{
		HR(app.getDevice().CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, &m_pixelShader));
	}
}
