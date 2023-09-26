#pragma once
#include "idrawable.h"

#include "constantbuffer.h"

#include <string>
#include <vector>
#include <memory>

namespace DirectX
{
	struct XMMATRIX;
}


namespace d3d
{
	class D3DApp;

	class D3DScene
	{
	public:

		void draw(D3DApp& app);

		void update(D3DApp& app, float deltaTime);

		void setWorldMatrix(D3DApp& app, const DirectX::XMMATRIX& worldMatrix);

		void setViewProjMatrix(D3DApp& app, const DirectX::XMMATRIX& projectionMatrix);

		D3DScene(D3DApp& app, const std::string& path);

		D3DScene() = default;

	private:

		std::unique_ptr<IBindable> m_camera{};

		std::vector<std::unique_ptr<IDrawable>> m_drawables{};

		ConstantBuffer<DirectX::XMMATRIX> m_worldMatCBuffer{};

		ConstantBuffer<DirectX::XMMATRIX> m_viewProjMatCBuffer{};
	};
}
