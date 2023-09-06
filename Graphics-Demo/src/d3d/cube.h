#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

#include "idrawable.h"
#include "ibindable.h"
#include "vertexshader.h"
#include "pixelshader.h"
#include "inputlayout.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "constantbuffer.h"

#include "vertex.h"

#include <vector>

namespace d3d
{
	using Microsoft::WRL::ComPtr;

	class Cube : public IDrawable
	{
	public:

		void draw(ID3D11DeviceContext& context) override;

		Cube(ID3D11Device& device, float aspectRatio);

		Cube() = default;

		~Cube() override = default;

	private:

		const float m_radToDeg{ 0.017453f };

		float m_roll{10.0f * m_radToDeg};
		float m_pitch{20.0f * m_radToDeg};
		float m_yaw{-10.0f * m_radToDeg};

		float m_aspect{ 3.0f / 3.0f };


		std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputDescs
		{
			{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"Color", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
		};

		Vertex m_vertices[8]
		{
			{-0.5f,	-0.5f,	-0.5f,	255u, 0u,	0u,   0u},
			{-0.5f,	0.5f,	-0.5f,	0u,   255u, 0u,   0u},
			{0.5f,	0.5f,	-0.5f,	0u,	  0u,	255u, 0u},
			{0.5f,	-0.5f,	-0.5f,	255u, 0u,	255u, 0u},

			{-0.5f,	-0.5f,	0.5f,	255u, 0u,	0u,   0u},
			{-0.5f,	0.5f,	0.5f,	0u,   255u, 0u,   0u},
			{0.5f,	0.5f,	0.5f,	0u,	  0u,	255u, 0u},
			{0.5f,	-0.5f,	0.5f,	255u, 0u,	255u, 0u}
		};

		const UINT16 m_indices[36]
		{
			0, 1, 2,	0, 2, 3,
			1, 5, 6,	1, 6, 2,
			0, 4, 5,	0, 5, 1,
			0, 3, 7,    0, 7, 4,
			2, 6, 7,	2, 7, 3,
			4, 5, 6,	4, 6, 7
		};
		
		DirectX::XMMATRIX m_transform
		{
			DirectX::XMMatrixTranspose
			(
			DirectX::XMMatrixRotationRollPitchYaw(m_roll, m_pitch, m_yaw) *
			DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f) *
			DirectX::XMMatrixPerspectiveLH(m_aspect, 1.0f, 2.0f, 100.0f)
			)
		};

		const UINT m_strides = sizeof(Vertex);

		const UINT m_offsets = 0;

		const wchar_t* m_vsShaderPath{ L"Debug/vertexshader.cso" };

		const wchar_t* m_pxshaderPath{ L"Debug/pixelshader.cso" };

		//Bindables

		VertexShader m_vertexShader{};

		PixelShader m_pixelShader{};

		InputLayout m_inputLayout{};

		VertexBuffer m_vertexBuffer{};

		IndexBuffer m_indexBuffer{};

		ConstantBuffer m_constantBuffer{};
	};
}