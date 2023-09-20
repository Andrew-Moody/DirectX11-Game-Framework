#pragma once
#include "idrawable.h"
#include "ibindable.h"
#include "vertexshader.h"
#include "pixelshader.h"
#include "inputlayout.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "constantbuffer.h"
#include "vertex.h"

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

#include <vector>

namespace DirectX
{
	class XMMatrix;
}


namespace d3d
{
	class D3DApp;
	using Microsoft::WRL::ComPtr;


	class Cube : public IDrawable
	{
	public:

		void draw(D3DApp& app) override;

		void update(D3DApp& app, float deltaTime);

		DirectX::XMMATRIX getTransform();

		Cube(D3DApp& app);

		Cube() = default;

		~Cube() override = default;

	private:

		float m_angleX{DirectX::XMConvertToRadians(20.0f) };
		float m_angleY{DirectX::XMConvertToRadians(-120.0f) };
		float m_angleZ{DirectX::XMConvertToRadians(0.0f) };

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
			4, 6, 5,	4, 7, 6
		};

		DirectX::XMFLOAT3 m_translation{ 0.0f, 0.0f, 5.0f };
		DirectX::XMFLOAT3 m_rotation{ m_angleX, m_angleY, m_angleZ };
		DirectX::XMFLOAT3 m_scale{ 1.0f, 1.0f, 1.0f };

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
	};
}