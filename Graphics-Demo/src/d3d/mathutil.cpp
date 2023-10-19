#include "mathutil.h"

#include <DirectXMath.h>
#include <assimp/scene.h>

#include <iostream>

#define LOGMAT

namespace d3d
{
	using namespace DirectX;

	void printMatrix(const DirectX::XMFLOAT4X4& matrix)
	{
#ifdef LOGMAT

		std::ios prevState(nullptr);
		prevState.copyfmt(std::cout);


		std::cout << matrix._11 << ", " << matrix._12 << ", " << matrix._13 << ", " << matrix._14 << '\n';
		std::cout << matrix._21 << ", " << matrix._22 << ", " << matrix._23 << ", " << matrix._24 << '\n';
		std::cout << matrix._31 << ", " << matrix._32 << ", " << matrix._33 << ", " << matrix._34 << '\n';
		std::cout << matrix._41 << ", " << matrix._42 << ", " << matrix._43 << ", " << matrix._44 << '\n';


		std::cout.copyfmt(prevState);
#endif
	}


	void printMatrix(const aiMatrix4x4& matrix)
	{
#ifdef LOGMAT
		std::cout << matrix.a1 << ", " << matrix.a2 << ", " << matrix.a3 << ", " << matrix.a4 << '\n';
		std::cout << matrix.b1 << ", " << matrix.b2 << ", " << matrix.b3 << ", " << matrix.b4 << '\n';
		std::cout << matrix.c1 << ", " << matrix.c2 << ", " << matrix.c3 << ", " << matrix.c4 << '\n';
		std::cout << matrix.d1 << ", " << matrix.d2 << ", " << matrix.d3 << ", " << matrix.d4 << '\n';
#endif
	}


	void printRotationMatrix(const aiMatrix3x3& matrix)
	{
#ifdef LOGMAT
		std::cout << matrix.a1 << ", " << matrix.a2 << ", " << matrix.a3 << '\n';
		std::cout << matrix.b1 << ", " << matrix.b2 << ", " << matrix.b3 << '\n';
		std::cout << matrix.c1 << ", " << matrix.c2 << ", " << matrix.c3 << '\n';
#endif
	}


	void printDecomp(const DirectX::XMFLOAT4X4& matrix)
	{
#ifdef LOGMAT
		constexpr float r2d{ 57.2958 };

		XMVECTOR scaleV{};
		XMVECTOR quatV{};
		XMVECTOR positionV{};

		XMMatrixDecompose(&scaleV, &quatV, &positionV, XMLoadFloat4x4(&matrix));

		XMFLOAT3 scale{};
		XMFLOAT4 quat{};
		XMFLOAT3 position{};

		XMStoreFloat3(&scale, scaleV);
		XMStoreFloat4(&quat, quatV);
		XMStoreFloat3(&position, positionV);


		XMFLOAT3 rotation{};

		rotation.x = asin(-matrix._32);
		rotation.y = atan2(matrix._31, matrix._33);
		rotation.z = atan2(matrix._12, matrix._22);

		// Used to help test euler calculation
		/*std::cout << "double check rotation decomp" << '\n';

		XMFLOAT4X4 rotationFromEuler{};

		XMStoreFloat4x4(&rotationFromEuler, XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z));

		printMatrix(rotationFromEuler);
		std::cout << '\n';*/


		//std::cout << "Scale: x: " << scale.x << ", y: " << scale.y << ", z: " << scale.z << '\n';
		std::cout << "Rotation: x: " << rotation.x * r2d << ", y: " << rotation.y * r2d << ", z: " << rotation.z * r2d << '\n';
		//std::cout << "Position: x: " << position.x << ", y: " << position.y << ", z: " << position.z << '\n';
		//std::cout << "Quaternion: w: " << quat.w << " x: " << quat.x << ", y : " << quat.y << ", z : " << quat.z << '\n';
		//std::cout << "Decomposed from XMFLOAT4X4" << '\n';

		// use this to test that the calculated euler angles are correct
		/*XMFLOAT4X4 test{};

		XMStoreFloat4x4(&test, XMMatrixRotationRollPitchYaw(1.0f, 2.0f, 3.0f));

		std::cout << "Decomposition Test" << '\n';
		printMatrix(test);
		printDecomp(test);
		std::cout << '\n';*/
#endif
	}


	void printDecomp(const aiMatrix4x4& matrix)
	{
#ifdef LOGMAT
		constexpr float r2d{ 57.2958 };

		aiVector3D scaling{};
		aiVector3D rotation{};
		aiVector3D position{};

		matrix.Decompose(scaling, rotation, position);

		aiQuaternion quat(rotation.y, rotation.z, rotation.x);

		std::cout << "Scale: x: " << scaling.x << ", y: " << scaling.y << ", z: " << scaling.z << '\n';
		std::cout << "Rotation: x: " << rotation.x * r2d << ", y: " << rotation.y * r2d << ", z: " << rotation.z * r2d << '\n';
		std::cout << "Position: x: " << position.x << ", y: " << position.y << ", z: " << position.z << '\n';
		std::cout << "Quaternion: w: " << quat.w << " x: " << quat.x << ", y : " << quat.y << ", z : " << quat.z << '\n';
		std::cout << "Decomposed from aiMatrix4x4" << '\n';
#endif
	}


	void normalizeWeights(XMFLOAT4& weights)
	{
		float correction = 1.0f / (weights.x + weights.y + weights.z + weights.w);

		weights.x *= correction;
		weights.y *= correction;
		weights.z *= correction;
		weights.w *= correction;
	}
}
