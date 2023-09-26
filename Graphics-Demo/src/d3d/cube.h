#pragma once
#include "idrawable.h"
#include "ibindable.h"

#include "transform.h"


namespace d3d
{
	class D3DApp;
	class Mesh;
	class Material;

	class Cube : public IDrawable
	{
	public:

		void draw(D3DApp& app) override;

		void update(D3DApp& app, float deltaTime);

		Cube(D3DApp& app, Mesh* mesh, Material* material);

		Cube() = default;

		//~Cube() override = default;

	private:

		Transform m_transform{};
		
		Mesh* m_mesh{};

		IBindable* m_material{};
	};
}