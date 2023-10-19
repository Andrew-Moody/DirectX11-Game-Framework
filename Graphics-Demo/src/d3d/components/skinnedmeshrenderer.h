#pragma once
#include "../component.h"
#include "../idrawable.h"

#include <DirectXMath.h>

#include <vector>
#include <string>

namespace tinyxml2
{
	class XMLElement;
}


namespace d3d
{
	class D3DApp;
	class ModelData;
	class Mesh;
	class Material;
	class SkeletonData;
	class AnimationData;
	class GameObject;


	class SkinnedMeshRenderer : public Component, public IDrawable
	{
	public:

		void draw(D3DApp& app) override;

		void update(float deltaTime) override;

		void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element) override;

		SkinnedMeshRenderer(GameObject& gameObject) : Component(gameObject) {}

	private:

		Material* m_material{};

		Mesh* m_mesh{};

		const ModelData* m_model{};

		const SkeletonData* m_skeleton{};

		const AnimationData* m_currentAnimation{};

		bool m_keyPressedPrevFrame{ false };

		float m_timeInAnim{};

		int m_currentAnimIndex{ 0 };

		std::vector<std::string> m_animNames{};

		std::vector<DirectX::XMFLOAT4X4> m_boneTransforms{};
	};
}
