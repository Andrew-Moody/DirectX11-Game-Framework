#include "d3dscene.h"
#include "d3dapp.h"
#include "idrawable.h"
#include "camera.h"
#include "cube.h"
#include "gameobject.h"
#include "assetloader.h"
#include "modeldata.h"
#include "mesh.h"

#include "bytecode.h"
#include "inputlayout.h"
#include "vertexshader.h"
#include "pixelshader.h"
#include "material.h"

#include <DirectXMath.h>

#include <string>
#include <vector>
#include <memory>

namespace d3d
{
	using namespace DirectX;

	D3DScene::D3DScene(D3DApp& app, const std::string& path)
		: m_worldMatCBuffer{ app, 0u, DirectX::XMMatrixIdentity() },
		m_viewProjMatCBuffer{ app, 1u, DirectX::XMMatrixIdentity() }
	{
		m_worldMatCBuffer.bind(app);

		m_viewProjMatCBuffer.bind(app);

		m_camera = std::make_unique<Camera>(app);

		app.getResourceManager().loadScene(app, path);

		Mesh* cubeMesh = app.getResourceManager().getCubeMesh();

		Material* material = app.getResourceManager().getMaterial();

		m_gameObjects.push_back(std::make_unique<GameObject>(app, cubeMesh, material));

		m_gameObjects[0]->addComponent(std::make_unique<Spin>(*(m_gameObjects[0].get())));

		Mesh* mesh = app.getResourceManager().getMesh();

		m_gameObjects.push_back(std::make_unique<GameObject>(app, mesh, material));

		m_gameObjects[1]->addComponent(std::make_unique<Spin>(*(m_gameObjects[1].get())));

		m_gameObjects[1]->setParent(m_gameObjects[0].get());
	}


	void D3DScene::draw(D3DApp& app)
	{
		m_camera->bind(app);

		app.getResourceManager().getInputLayout()->bind(app);

		app.getResourceManager().getMaterial()->bind(app);

		app.getResourceManager().getSamplerState()->bind(app);

		app.getResourceManager().getTexture()->bind(app);


		for (auto& gameObject : m_gameObjects)
		{
			gameObject->draw(app);
		}
	}


	void D3DScene::update(D3DApp& app, float deltaTime)
	{
		dynamic_cast<Camera*>(m_camera.get())->update(app, deltaTime);

		for (auto& gameObject : m_gameObjects)
		{
			gameObject->update(app, deltaTime);
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
