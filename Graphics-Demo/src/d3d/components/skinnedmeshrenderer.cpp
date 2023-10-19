#include "skinnedmeshrenderer.h"

#include "../material.h"
#include "../mesh.h"
#include "../modeldata.h"
#include "../skeletondata.h"
#include "../animationdata.h"

#include "../d3dapp.h"
#include "../d3dutil.h"

#include <tinyxml2.h>

#include <iostream>
#include <vector>
#include <string>


namespace d3d
{
	void SkinnedMeshRenderer::draw(D3DApp& app)
	{
		// Retrieve the bone transform matrices from the scene
		std::array<DirectX::XMMATRIX, 64>& boneTransforms = app.getScene().getBoneTransforms();

		// Update the bone transform matrices
		for (size_t i = 0; i < m_boneTransforms.size(); ++i)
		{
			boneTransforms[i] = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_boneTransforms[i]));
		}

		// Apply the changes to the constant buffer
		app.getScene().setBoneTransCBuffer(app, boneTransforms);


		// Draw as normal
		app.getScene().setWorldMatrix(app, m_transform.getTransformMatrix());

		m_material->bind(app);

		m_mesh->draw(app);
	}


	void SkinnedMeshRenderer::update(float deltaTime)
	{
		// Handle Input
		if (GetAsyncKeyState('R') & 0x8000)
		{
			if (!m_keyPressedPrevFrame)
			{
				m_keyPressedPrevFrame = true;

				m_currentAnimIndex++;

				if (m_currentAnimIndex >= m_animNames.size())
				{
					m_currentAnimIndex = 0;
				}

				m_currentAnimation = m_model->getAnimation(m_animNames[m_currentAnimIndex]);
			}

		}
		else
		{
			m_keyPressedPrevFrame = false;
		}

		m_timeInAnim += deltaTime * 0.001f;

		if (m_currentAnimation)
		{
			m_skeleton->getFinalTransforms(m_timeInAnim, m_currentAnimation, m_boneTransforms);
		}
	}


	void SkinnedMeshRenderer::deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element)
	{
		using namespace tinyxml2;

		DB_ASSERT(strcmp(element->Attribute("type"), "SkinnedMeshRenderer") == 0);

		const XMLElement* materialElement = element->FirstChildElement("Material");

		const XMLElement* modelElement = element->FirstChildElement("Model");

		if (!materialElement)
		{
			std::cout << "Material missing from gameObject: " << element->Attribute("id") << '\n';
			return;
		}

		if (!modelElement)
		{
			std::cout << "Model missing from gameObject: " << element->Attribute("id") << '\n';
			return;
		}

		m_material = dynamic_cast<Material*>(app.getResourceManager().getResource(materialElement->Attribute("id")));

		if (!m_material)
		{
			std::cout << "Missing Material for SkinnedMeshRender: " << element->Attribute("id") << '\n';
			return;
		}

		m_model = dynamic_cast<ModelData*>(app.getResourceManager().getResource(modelElement->Attribute("id")));

		if (!m_model)
		{
			std::cout << "Missing Model for SkinnedMeshRender: " << element->Attribute("id") << '\n';
			return;
		}

		m_mesh = m_model->getMesh(0);

		if (!m_mesh)
		{
			std::cout << "Missing Mesh for SkinnedMeshRender: " << element->Attribute("id") << '\n';
			return;
		}

		m_skeleton = m_model->getSkeleton(0);

		if (!m_skeleton)
		{
			std::cout << "Missing Skeleton for SkinnedMeshRender: " << element->Attribute("id") << '\n';
			return;
		}


		const XMLElement* animElement = element->FirstChildElement("Animations");

		if (animElement)
		{
			const XMLElement* current = animElement->FirstChildElement();

			while (current)
			{
				m_animNames.push_back(current->Attribute("name"));

				current = current->NextSiblingElement();
			}
		}

		if (m_animNames.size() < 1)
		{
			std::cout << "No Animations found for SkinnedMeshRender: " << element->Attribute("id") << '\n';
			return;
		}

		m_currentAnimation = m_model->getAnimation(m_animNames[0]);
	}
}
