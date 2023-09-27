#include "transform.h"

#include <DirectXMath.h>

#include <iostream>


namespace d3d
{
	using namespace DirectX;

	XMMATRIX Transform::getTransformMatrix() const
	{
		return XMLoadFloat4x4(&m_parentTransform) * XMMatrixTranspose(
			XMMatrixScaling(m_localTransformData.scaleX, m_localTransformData.scaleY, m_localTransformData.scaleZ) *
			XMMatrixRotationRollPitchYaw(m_localTransformData.rotX, m_localTransformData.rotY, m_localTransformData.rotZ) *
			XMMatrixTranslation(m_localTransformData.posX, m_localTransformData.posY, m_localTransformData.posZ)
		);
	}

	void Transform::translate(float x, float y, float z)
	{
		m_localTransformData.posX += x;
		m_localTransformData.posY += y;
		m_localTransformData.posZ += z;
	}

	void Transform::rotate(float x, float y, float z)
	{
		m_localTransformData.rotX += DirectX::XMConvertToRadians(x);
		m_localTransformData.rotY += DirectX::XMConvertToRadians(y);
		m_localTransformData.rotZ += DirectX::XMConvertToRadians(z);
	}


	void Transform::updateParentTransform(const Transform& parent)
	{
		XMStoreFloat4x4(&m_parentTransform, parent.getTransformMatrix());
	}


	Transform::Transform(const TransformData& transformData)
		: m_localTransformData{ transformData }
	{
		XMStoreFloat4x4(&m_parentTransform, XMMatrixIdentity());
	}


	Transform::Transform()
	{
		XMStoreFloat4x4(&m_parentTransform, XMMatrixIdentity());
	}


	void Transform::printMatrix(const XMMATRIX& matrix) const
	{
		XMFLOAT4X4 float4x4;

		XMStoreFloat4x4(&float4x4, matrix);

		printMatrix(float4x4);
	}

	void Transform::printMatrix(const XMFLOAT4X4& matrix) const
	{
		for (size_t i = 0; i < 4; ++i)
		{
			for (size_t j = 0; j < 4; ++j)
			{
				std::cout << m_parentTransform(i, j) << " ";
			}

			std::cout << '\n';
		}

		std::cout << '\n';
	}
}
