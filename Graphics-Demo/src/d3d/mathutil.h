#pragma once

#include <assimp/scene.h>

namespace DirectX
{
	struct XMFLOAT4X4;
	struct XMFLOAT4;
}


namespace d3d
{
	void printMatrix(const DirectX::XMFLOAT4X4& matrix);

	void printMatrix(const aiMatrix4x4& matrix);

	void printRotationMatrix(const aiMatrix3x3& matrix);

	void printDecomp(const DirectX::XMFLOAT4X4& matrix);

	void printDecomp(const aiMatrix4x4& matrix);

	void normalizeWeights(DirectX::XMFLOAT4& weights);
}
