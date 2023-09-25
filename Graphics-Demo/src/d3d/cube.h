#pragma once
#include "idrawable.h"
#include "ibindable.h"

#include "transform.h"
#include "mesh.h"


namespace d3d
{
	class D3DApp;
	class Material;

	class Cube : public IDrawable
	{
	public:

		void draw(D3DApp& app) override;

		void update(D3DApp& app, float deltaTime);

		Cube(D3DApp& app, Material* material);

		Cube() = default;

		~Cube() override = default;

	private:

		Transform m_transform{};
		
		Mesh m_mesh{};

		IBindable* m_material{};
	};
}