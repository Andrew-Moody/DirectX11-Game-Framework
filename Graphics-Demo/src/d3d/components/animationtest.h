#pragma once
#include "../component.h"
#include "../d3dapp.h"
#include "../gameobject.h"
#include "../transform.h"

#include "../skeletondata.h"
#include "../animationdata.h"

#include <DirectXMath.h>

#include <iostream>
#include <string>

namespace d3d
{
	class GameObject;
	class Transform;


	class AnimationTest : public Component
	{
	public:

		void update(float deltaTime) override
		{
			if (GetAsyncKeyState('R') & 0x8000)
			{
				if (!m_keyPressedPrevFrame)
				{
					m_keyPressedPrevFrame = true;

					m_currentAnim++;

					if (m_currentAnim >= m_animNames.size())
					{
						m_currentAnim = 0;
					}
				}
				
			}
			else
			{
				m_keyPressedPrevFrame = false;
			}

			m_timeInAnim += deltaTime * 0.001f;

			D3DApp& app = m_gameObject.getApp();

			ModelData* model = app.getResourceManager().getModel();
			
			const SkeletonData* skeleton = model->getSkeleton(0);
			const AnimationData* animation = model->getAnimation(m_animNames[m_currentAnim]);

			if (!skeleton || !animation)
			{
				if (!skeleton)
				{
					std::cout << "Model does not support skeletal animation" << '\n';
				}
				
				if (!animation)
				{
					std::cout << "Model does not have an animation with the name: " << m_animNames[m_currentAnim] << '\n';
				}
				return;
			}


			skeleton->getFinalTransforms(m_timeInAnim, animation, m_boneTransforms);


			// Retrieve the bone transform matrices from the scene
			std::array<DirectX::XMMATRIX, 64>& boneTransforms = app.getScene().getBoneTransforms();

			// Update the bone transform matrices
			for (size_t i = 0; i < m_boneTransforms.size(); ++i)
			{
				// Untransposed
				//boneTransforms[i] = DirectX::XMLoadFloat4x4(&m_boneTransforms[i]);

				//transposed
				boneTransforms[i] = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_boneTransforms[i]));
			}

			// Apply the changes to the constant buffer
			app.getScene().setBoneTransCBuffer(app, boneTransforms);
		}


		AnimationTest(GameObject& gameObject)
			: Component(gameObject)
		{

		}

	private:

		bool m_keyPressedPrevFrame{ false };

		bool m_hasPrinted{ false };

		float m_timeInAnim{};

		int m_currentAnim{ 0 };
		//std::vector<std::string> m_animNames{ "Armature|TPose", "Armature|RotateX", "Armature|RotateZ" };
		std::vector<std::string> m_animNames{ "ManRig|0ManTPose", "ManRig|ManIdle", "ManRig|ManWalk", "ManRig|ManSlash" };

		std::vector<DirectX::XMFLOAT4X4> m_boneTransforms{};
	};
}
