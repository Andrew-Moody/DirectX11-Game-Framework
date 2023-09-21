#include "cube.h"
#include "idrawable.h"
#include "ibindable.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "constantbuffer.h"
#include "d3dapp.h"
#include "d3dutil.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

#include <sstream>


namespace d3d
{
	Cube::Cube(D3DApp& app)
		: m_vertexShader{ app, m_vsShaderPath},
		m_pixelShader{ app, m_pxshaderPath},
		m_inputLayout{ app, m_inputDescs, m_vertexShader.GetByteCode()},
		m_vertexBuffer{ app, m_vertices, sizeof(m_vertices), sizeof(Vertex)},
		m_indexBuffer{ app, m_indices, sizeof(m_indices)}
	{
		DB_LOG("Constructing Cube");
	}


	void Cube::draw(D3DApp& app)
	{
		app.getScene().setWorldMatrix(app, getTransform());

		m_vertexShader.bind(app);

		m_pixelShader.bind(app);

		m_inputLayout.bind(app);

		app.getContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_vertexBuffer.bind(app);

		m_indexBuffer.bind(app);

		app.getContext().DrawIndexed(std::size(m_indices), 0u, 0);
	}


	void Cube::update(D3DApp& app, float deltaTime)
	{
		const float deltaSeconds = 0.001f * deltaTime;

		constexpr float xSpeed = 0.2f;
		constexpr float ySpeed = 0.5f;
		constexpr float zSpeed = 0.1f;

		m_rotation.x += deltaSeconds * xSpeed;
		m_rotation.y += deltaSeconds * ySpeed;
		m_rotation.z += deltaSeconds * zSpeed;
	}


	DirectX::XMMATRIX Cube::getTransform()
	{
		return DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&m_rotation)) *
			DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&m_scale)) *
			DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&m_translation))
		);
	}
}
