#include "vertexshader.h"

#include "bytecode.h"

#include "d3dutil.h"

namespace d3d
{
	void VertexShader::bind(ID3D11DeviceContext& context)
	{
		DB_LOG("Binding VertexShader");

		context.VSSetShader(m_vertexShader.Get(), nullptr, 0u);
	}


	VertexShader::VertexShader(ID3D11Device& device, const wchar_t* filepath)
		: m_bytecode{ CreateShaderByteCode(filepath) }
	{
		HR(device.CreateVertexShader(m_bytecode.data(), m_bytecode.size(), nullptr, &m_vertexShader));
	}
}