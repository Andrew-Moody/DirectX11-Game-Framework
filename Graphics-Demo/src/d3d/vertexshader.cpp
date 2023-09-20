#include "vertexshader.h"
#include "d3dapp.h"
#include "bytecode.h"

#include "d3dutil.h"

namespace d3d
{
	void VertexShader::bind(D3DApp& app)
	{
		DB_LOG("Binding VertexShader");

		app.getContext().VSSetShader(m_vertexShader.Get(), nullptr, 0u);
	}


	VertexShader::VertexShader(D3DApp& app, const wchar_t* filepath)
		: m_bytecode{ CreateShaderByteCode(filepath) }
	{
		HR(app.getDevice().CreateVertexShader(m_bytecode.data(), m_bytecode.size(), nullptr, &m_vertexShader));
	}
}