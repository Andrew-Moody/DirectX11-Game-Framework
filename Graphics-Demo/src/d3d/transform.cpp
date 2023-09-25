#include "transform.h"


#include <DirectXMath.h>

namespace d3d
{
	using namespace DirectX;

	XMMATRIX Transform::getTransformMatrix() const
	{
		return XMMatrixTranspose(
			XMMatrixScaling(m_transformData.scaleX, m_transformData.scaleY, m_transformData.scaleZ) *
			XMMatrixRotationRollPitchYaw(m_transformData.rotX, m_transformData.rotY, m_transformData.rotZ) *
			XMMatrixTranslation(m_transformData.posX, m_transformData.posY, m_transformData.posZ)
		);
	}

	void Transform::translate(float x, float y, float z)
	{
		m_transformData.posX += x;
		m_transformData.posY += y;
		m_transformData.posZ += z;
	}

	void Transform::rotate(float x, float y, float z)
	{
		m_transformData.rotX += DirectX::XMConvertToRadians(x);
		m_transformData.rotY += DirectX::XMConvertToRadians(y);
		m_transformData.rotZ += DirectX::XMConvertToRadians(z);
	}


	Transform::Transform(const TransformData& transformData)
		: m_transformData{ transformData }
	{

	}
}
