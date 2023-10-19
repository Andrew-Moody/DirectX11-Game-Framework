#include "d3dscene.h"
#include "d3dapp.h"
#include "gameobject.h"
#include "components/camera.h"

#include <DirectXMath.h>

#include <string>
#include <vector>
#include <memory>
#include <iostream>


namespace d3d
{
	using namespace DirectX;

	D3DScene::D3DScene(D3DApp& app, const std::string& path)
		: m_worldMatCBuffer{ app, 0u, DirectX::XMMatrixIdentity() },
		m_viewProjMatCBuffer{ app, 1u, DirectX::XMMatrixIdentity() },
		m_boneTransCBuffer{app, 2u, *m_boneTransforms.data() }
	{
		m_worldMatCBuffer.bind(app);

		m_viewProjMatCBuffer.bind(app);

		m_boneTransCBuffer.bind(app);

		m_gameObjects = app.getResourceManager().loadSceneXML(app, path);

		for (auto& gameObject : m_gameObjects)
		{
			if (gameObject->getID() == "Camera")
			{
				Component* cameraComponent = gameObject->getComponent("Camera");

				m_camera = dynamic_cast<Camera*>(cameraComponent);

				if (m_camera)
				{
					break;
				}
				else
				{
					std::cout << "Camera GameObject missing Camera Component" << '\n';
				}
			}
		}
	}


	void D3DScene::draw(D3DApp& app)
	{
		if (m_camera)
		{
			m_viewProjMatCBuffer.setData(app, m_camera->getViewProj());
		}
		

		for (auto& gameObject : m_gameObjects)
		{
			gameObject->draw(app);
		}
	}


	void D3DScene::update(D3DApp& app, float deltaTime)
	{
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


	void D3DScene::setBoneTransCBuffer(D3DApp& app, const std::array<DirectX::XMMATRIX, 64>& boneTransforms)
	{
		m_boneTransCBuffer.setData(app, *boneTransforms.data());
	}
}
