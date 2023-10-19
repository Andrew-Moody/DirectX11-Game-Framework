#pragma once
#include "../component.h"

#include <DirectXMath.h>

namespace d3d
{
	class GameObject;


	class Camera : public Component
	{
	public:

		void update(float deltaTime) override;

		void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element) override;

		DirectX::XMMATRIX getViewProj() const;

		void setFrustum(float fov, float aspect, float nearZ, float farZ);

		Camera(GameObject& gameObject) : Component{gameObject} {}

	private:

		float m_fov{ DirectX::XMConvertToRadians(90.0f) };
		float m_aspect{ 1.0f };
		float m_nearZ{ 0.1f };
		float m_farZ{ 100.0f };

		DirectX::XMFLOAT4X4 m_perspectiveMatrix{};
	};
}
