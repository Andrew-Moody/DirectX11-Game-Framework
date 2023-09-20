#pragma once
#include "idrawable.h"

#include "constantbuffer.h"

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

		D3DScene(D3DApp& app, const char* path);

		D3DScene() = default;

	private:

		std::vector<std::unique_ptr<IDrawable>> m_drawables{};

		ConstantBuffer<DirectX::XMMATRIX> m_worldMatCBuffer{};
	};
}
