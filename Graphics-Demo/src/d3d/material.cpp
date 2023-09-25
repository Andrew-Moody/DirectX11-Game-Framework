#include "material.h"

#include "vertexshader.h"
#include "pixelshader.h"

#include "d3dapp.h"

namespace d3d
{
	void Material::bind(D3DApp& app)
	{
		m_vertexShader->bind(app);

		m_pixelShader->bind(app);
	}


	Material::Material(VertexShader* vertexShader, PixelShader* pixelShader)
		: m_vertexShader{ vertexShader }, m_pixelShader{ pixelShader }
	{

	}
}
