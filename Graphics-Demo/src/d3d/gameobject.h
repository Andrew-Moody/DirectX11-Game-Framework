#pragma once
#include "iserializable.h"
#include "transform.h"
#include "component.h"

#include <vector>
#include <memory>

namespace d3d
{
	class D3DApp;
	class Mesh;
	class Material;
	class ModelData;

	class GameObject : public ISerializable
	{
	public:

		void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element) override;

		void draw(D3DApp& app);

		void update(D3DApp& app, float deltaTime);

		void setParent(GameObject* parent);

		void addComponent(std::unique_ptr<Component> component);

		Transform& getTransform() { return m_transform; }

		D3DApp* getApp() { return m_app; }

		ModelData* getModelData() { return m_model; }

		GameObject(D3DApp& app, Mesh* mesh, Material* material);

		GameObject() = default;

	private:

		D3DApp* m_app;

		Transform m_transform{};

		Mesh* m_mesh{};

		Material* m_material{};

		ModelData* m_model{};

		std::vector<std::unique_ptr<Component>> m_components;

		GameObject* m_parent{};

		std::vector<GameObject*> m_children{};
	};
}
