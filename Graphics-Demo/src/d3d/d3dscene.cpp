#include "d3dscene.h"
#include "d3dapp.h"
#include "idrawable.h"
#include "camera.h"
#include "cube.h"
#include "assetloader.h"
#include "modeldata.h"
#include "mesh.h"

#include <DirectXMath.h>

#include <vector>
#include <memory>

namespace d3d
{
	using namespace DirectX;

	D3DScene::D3DScene(D3DApp& app, const char* path)
		: m_worldMatCBuffer{ app, 0u, DirectX::XMMatrixIdentity() },
		m_viewProjMatCBuffer{ app, 1u, DirectX::XMMatrixIdentity() }
	{
		m_worldMatCBuffer.bind(app);

		m_viewProjMatCBuffer.bind(app);

		m_camera = std::make_unique<Camera>(Camera(app));

		m_drawables.push_back(std::make_unique<Cube>(Cube(app)));


		AssetLoader assetLoader;

		ModelData model = assetLoader.loadModel("C:/Users/Andrew/Documents/GameProject/FBX/Axe.fbx");

		m_drawables.push_back(std::make_unique<Mesh>(model.getMesh(app, 0)));
	}


	void D3DScene::draw(D3DApp& app)
	{
		m_camera->bind(app);

		for (auto iter = m_drawables.begin(); iter != m_drawables.end(); ++iter)
		{
			(*iter)->draw(app);
		}
	}


	void D3DScene::update(D3DApp& app, float deltaTime)
	{
		dynamic_cast<Camera*>(m_camera.get())->update(app, deltaTime);

		for (auto iter = m_drawables.begin(); iter != m_drawables.end(); ++iter)
		{
			(*iter)->update(app, deltaTime);
		}
	}


	void D3DScene::setWorldMatrix(D3DApp& app, const DirectX::XMMATRIX& worldMatrix)
	{
		m_worldMatCBuffer.setData(app, worldMatrix);
	}


	void D3DScene::setViewProjMatrix(D3DApp& app, const DirectX::XMMATRIX& projectionMatrix)
	{
		m_viewProjMatCBuffer.setData(app, projectionMatrix);
	}
}
