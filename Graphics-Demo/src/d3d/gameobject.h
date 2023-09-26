#pragma once
#include "idrawable.h"
#include "transform.h"
#include "component.h"

#include "components/spin.h"

#include <vector>
#include <memory>

namespace d3d
{
	class D3DApp;
	class Mesh;
	class Material;

	class GameObject : public IDrawable
	{
	public:

		void draw(D3DApp& app);

		void update(D3DApp& app, float deltaTime);

		GameObject(D3DApp& app, Mesh* mesh, Material* material);

		GameObject() = default;

	private:

		Transform m_transform{};

		Mesh* m_mesh{};

		Material* m_material{};

		std::vector<std::unique_ptr<Component>> m_components;
	};
}
