#include "vertexshader.h"
#include "d3dapp.h"
#include "d3dutil.h"

#include <vector>

namespace d3d
{
	void VertexShader::bind(D3DApp& app)
	{
		DB_LOG("Binding VertexShader");

		app.getContext().VSSetShader(m_vertexShader.Get(), nullptr, 0u);
	}


	VertexShader::VertexShader(D3DApp& app, const std::vector<uint8_t>& byteCode)
	{
		HR(app.getDevice().CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, &m_vertexShader));
	}
}
