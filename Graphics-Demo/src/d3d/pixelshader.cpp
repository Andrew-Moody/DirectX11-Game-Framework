#include "pixelshader.h"

#include <vector>
#include "bytecode.h"

#include "d3dutil.h"

namespace d3d
{
	void PixelShader::bind(ID3D11DeviceContext& context)
	{
		DB_LOG("Binding PixelShader");

		context.PSSetShader(m_pixelShader.Get(), nullptr, 0u);
	}


	PixelShader::PixelShader(ID3D11Device& device, const wchar_t* filepath)
	{
		std::vector<char> bytecode = CreateShaderByteCode(filepath);

		HR(device.CreatePixelShader(bytecode.data(), bytecode.size(), nullptr, &m_pixelShader));
	}
}
