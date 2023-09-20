#include "pixelshader.h"
#include "d3dapp.h"
#include "bytecode.h"
#include "d3dutil.h"
#include <vector>


namespace d3d
{
	void PixelShader::bind(D3DApp& app)
	{
		DB_LOG("Binding PixelShader");

		app.getContext().PSSetShader(m_pixelShader.Get(), nullptr, 0u);
	}


	PixelShader::PixelShader(D3DApp& app, const wchar_t* filepath)
	{
		std::vector<char> bytecode = CreateShaderByteCode(filepath);

		HR(app.getDevice().CreatePixelShader(bytecode.data(), bytecode.size(), nullptr, &m_pixelShader));
	}
}
