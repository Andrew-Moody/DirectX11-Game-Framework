#include "pixelshader.h"
#include "d3dapp.h"
#include "d3dutil.h"

#include <vector>


namespace d3d
{
	void PixelShader::bind(D3DApp& app)
	{
		DB_LOG("Binding PixelShader");

		app.getContext().PSSetShader(m_pixelShader.Get(), nullptr, 0u);
	}


	PixelShader::PixelShader(D3DApp& app, const std::vector<uint8_t>& byteCode)
	{
		HR(app.getDevice().CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, &m_pixelShader));
	}
}
