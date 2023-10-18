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

#include "components/spin.h"
#include "components/animationtest.h"

#include <DirectXMath.h>

#include <string>
#include <vector>
#include <memory>


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

		// Eventually want to turn Camera into a gameObject
		m_camera = std::make_unique<Camera>(app);

		m_gameObjects = app.getResourceManager().loadSceneXML(app, path);
	}


	void D3DScene::draw(D3DApp& app)
	{
		m_camera->bind(app);

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


	void D3DScene::setBoneTransCBuffer(D3DApp& app, const std::array<DirectX::XMMATRIX, 64>& boneTransforms)
	{
		m_boneTransCBuffer.setData(app, *boneTransforms.data());
	}
}
