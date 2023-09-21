#include "camera.h"

#include "d3dapp.h"

#include <DirectXMath.h>

namespace d3d
{
	using namespace DirectX;

	void Camera::bind(D3DApp& app)
	{
		app.getScene().setViewProjMatrix(app, getViewProj());
	}


	void Camera::update(D3DApp& app, float deltaTime)
	{
		constexpr float speed{ 1.0f * 0.001f };

		if (GetAsyncKeyState('W') & 0x8000)
		{
			m_position.z -= speed * deltaTime;
		}
		else if (GetAsyncKeyState('A') & 0x8000)
		{
			m_position.x += speed * deltaTime;
		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{
			m_position.z += speed * deltaTime;
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			m_position.x -= speed * deltaTime;
		}
		else if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			m_position.y -= speed * deltaTime;
		}
		else if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		{
			m_position.y += speed * deltaTime;
		}
	}


	DirectX::XMMATRIX Camera::getViewProj() const
	{
		return XMMatrixTranspose(
			XMMatrixTranslation(m_position.x, m_position.y, m_position.z) *
			XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
			XMMatrixPerspectiveFovLH(m_fov, m_aspect, m_nearZ, m_farZ)
		);
	}


	void Camera::setFrustum(float fov, float aspect, float nearZ, float farZ)
	{
		m_fov = fov;
		m_aspect = aspect;
		m_nearZ = nearZ;
		m_farZ = farZ;
	}


	Camera::Camera(D3DApp& app)
		: m_aspect(app.getAspect())
	{

	}
}
