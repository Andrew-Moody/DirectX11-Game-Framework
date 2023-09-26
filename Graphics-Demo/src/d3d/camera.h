#pragma once
#include "ibindable.h"

#include <DirectXMath.h>

namespace d3d
{
	class D3DApp;

	class Camera : public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		void update(D3DApp& app, float deltaTime);

		DirectX::XMMATRIX getViewProj() const;

		void setFrustum(float fov, float aspect, float nearZ, float farZ);

		Camera(D3DApp& app);

		Camera() = default;

	private:

		float m_fov{ DirectX::XMConvertToRadians(90.0f) };
		float m_aspect{ 1.0f };
		float m_nearZ{ 0.1f };
		float m_farZ{ 100.0f };


		DirectX::XMFLOAT3 m_position{0.0f, 0.0f, 1.5f};
		DirectX::XMFLOAT3 m_rotation{0.0f, 0.0f, 0.0f};

		DirectX::XMFLOAT4X4 m_viewMatrix{};

		DirectX::XMFLOAT4X4 M_projMatrix{};
	};
}