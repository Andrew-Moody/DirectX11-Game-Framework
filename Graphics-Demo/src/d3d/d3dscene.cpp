#include "d3dscene.h"
#include "d3dapp.h"
#include "idrawable.h"
#include "cube.h"

#include <DirectXMath.h>

#include <vector>
#include <memory>

namespace d3d
{
	using namespace DirectX;

	D3DScene::D3DScene(D3DApp& app, const char* path)
		: m_worldMatCBuffer{ app, DirectX::XMMatrixIdentity() }
	{
		m_drawables.push_back(std::make_unique<Cube>(Cube(app)));

		m_worldMatCBuffer.bind(app);
	}


	void D3DScene::draw(D3DApp& app)
	{
		for (auto iter = m_drawables.begin(); iter != m_drawables.end(); ++iter)
		{
			(*iter)->draw(app);
		}
	}


	void D3DScene::update(D3DApp& app, float deltaTime)
	{
		for (auto iter = m_drawables.begin(); iter != m_drawables.end(); ++iter)
		{
			(*iter)->update(app, deltaTime);
		}
	}


	void D3DScene::setWorldMatrix(D3DApp& app, const DirectX::XMMATRIX& worldMatrix)
	{
		m_worldMatCBuffer.setData(app, worldMatrix);
	}
}
