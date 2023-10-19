#pragma once
#include "gameobject.h"

#include "constantbuffer.h"

#include <string>
#include <vector>
#include <array>
#include <memory>

namespace DirectX
{
	struct XMMATRIX;
}


namespace d3d
{
	class D3DApp;
	class Camera;

	class D3DScene
	{
	public:

		void draw(D3DApp& app);

		void update(D3DApp& app, float deltaTime);

		void setWorldMatrix(D3DApp& app, const DirectX::XMMATRIX& worldMatrix);

		void setViewProjMatrix(D3DApp& app, const DirectX::XMMATRIX& projectionMatrix);

		void setBoneTransCBuffer(D3DApp& app, const std::array<DirectX::XMMATRIX, 64>& boneTransforms);

		std::array<DirectX::XMMATRIX, 64>& getBoneTransforms() { return m_boneTransforms; }

		D3DScene(D3DApp& app, const std::string& path);

		D3DScene() = default;

	private:

		Camera* m_camera{};

		//std::vector<std::unique_ptr<GameObject>> m_gameObjects{};
		std::vector<GameObject*> m_gameObjects{};

		std::array<DirectX::XMMATRIX, 64> m_boneTransforms{};

		ConstantBuffer<DirectX::XMMATRIX, 1> m_worldMatCBuffer{};

		ConstantBuffer<DirectX::XMMATRIX, 1> m_viewProjMatCBuffer{};

		ConstantBuffer<DirectX::XMMATRIX, 64> m_boneTransCBuffer{};
	};
}
