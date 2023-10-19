#include "camera.h"

#include "../transform.h"

#include "../d3dapp.h"
#include "../d3dutil.h"

#include <tinyxml2.h>

namespace d3d
{
	using namespace DirectX;


	void Camera::update(float deltaTime)
	{
		constexpr float speed{ 1.0f * 0.001f };
		constexpr float rotSpeed{ 1.0f * 0.001f };

		float x{};
		float y{};
		float z{};

		float xRot{};
		float yRot{};
		float zRot{};

		if (GetAsyncKeyState('W') & 0x8000)
		{
			z -= speed * deltaTime;
		}
		else if (GetAsyncKeyState('A') & 0x8000)
		{
			x += speed * deltaTime;
		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{
			z += speed * deltaTime;
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			x -= speed * deltaTime;
		}
		else if (GetAsyncKeyState('Q') & 0x8000)
		{
			yRot += rotSpeed * deltaTime;
		}
		else if (GetAsyncKeyState('E') & 0x8000)
		{
			yRot -= rotSpeed * deltaTime;
		}
		else if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			y -= speed * deltaTime;
		}
		else if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		{
			y += speed * deltaTime;
		}
		
		m_transform.translate(x, y, z);
		m_transform.rotate(xRot, yRot, zRot);
	}


	void Camera::deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element)
	{
		using namespace tinyxml2;

		DB_ASSERT(strcmp(element->Attribute("type"), "Camera") == 0);

		const XMLElement* frustumElement = element->FirstChildElement("Frustum");

		if (frustumElement)
		{
			float fov = frustumElement->FloatAttribute("fov");
			float aspect = frustumElement->FloatAttribute("aspect");
			float nearZ = frustumElement->FloatAttribute("nearZ");
			float farZ = frustumElement->FloatAttribute("farZ");

			setFrustum(fov, aspect, nearZ, farZ);
		}
	}


	DirectX::XMMATRIX Camera::getViewProj() const
	{
		return XMLoadFloat4x4(&m_perspectiveMatrix) * m_transform.getTransformMatrix();

		//return m_transform.getTransformMatrix() * XMLoadFloat4x4(&m_perspectiveMatrix);
	}


	void Camera::setFrustum(float fov, float aspect, float nearZ, float farZ)
	{
		m_fov = fov;
		m_aspect = aspect;
		m_nearZ = nearZ;
		m_farZ = farZ;

		XMStoreFloat4x4(&m_perspectiveMatrix,
			XMMatrixTranspose(
				XMMatrixPerspectiveFovLH(m_fov, m_aspect, m_nearZ, m_farZ)
			)
		);
	}
}
