#include "cube.h"

#include <d3d11.h>
#include <wrl.h>

#include "d3dutil.h"

#include "idrawable.h"
#include "ibindable.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "constantbuffer.h"

#include <sstream>

//#include <utility>

namespace d3d
{
	Cube::Cube(ID3D11Device& device, float aspectRatio)
		: m_aspect{aspectRatio},
		m_vertexShader{device, m_vsShaderPath},
		m_pixelShader{device, m_pxshaderPath},
		m_inputLayout{device, m_inputDescs, m_vertexShader.GetByteCode()},
		m_vertexBuffer{device, m_vertices, sizeof(m_vertices), sizeof(Vertex)},
		m_indexBuffer{device, m_indices, sizeof(m_indices)},
		m_constantBuffer{device, &m_transform, sizeof(m_transform)}
	{
		DB_LOG("Constructing Cube");
	}


	void Cube::draw(ID3D11DeviceContext& context)
	{
		m_vertexShader.bind(context);

		m_pixelShader.bind(context);

		m_inputLayout.bind(context);

		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_vertexBuffer.bind(context);

		m_indexBuffer.bind(context);

		m_constantBuffer.bind(context);

		context.DrawIndexed(std::size(m_indices), 0u, 0);
	}
}
