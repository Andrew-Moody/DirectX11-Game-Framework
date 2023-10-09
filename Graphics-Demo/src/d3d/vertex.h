#pragma once

#include <DirectXMath.h>

namespace d3d
{
	struct SimpleVertex
	{
		float x;
		float y;
		float z;

		float u;
		float v;
	};


	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TexCoord;
		DirectX::XMFLOAT4 Tangent;
		DirectX::XMFLOAT4 Weights;
		uint8_t BoneIndices[4];
	};


	struct Vertex8Weights
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TexCoord;
		DirectX::XMFLOAT4 Tangent;
		DirectX::XMFLOAT4 Weights;
		uint8_t BoneIndices[4];
		DirectX::XMFLOAT4 Weights2;
		uint8_t BoneIndices2[4];
	};
}