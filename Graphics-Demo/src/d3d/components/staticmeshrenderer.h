#pragma once
#include "../component.h"
#include "../idrawable.h"

namespace tinyxml2
{
	class XMLElement;
}


namespace d3d
{
	class D3DApp;
	class Mesh;
	class Material;
	class GameObject;


	class StaticMeshRenderer : public Component, public IDrawable
	{
	public:

		void draw(D3DApp& app) override;

		void update(float deltaTime) override;

		void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element) override;

		StaticMeshRenderer(GameObject& gameObject) : Component(gameObject) {}

	private:

		Mesh* m_mesh{};

		Material* m_material{};
	};
}
