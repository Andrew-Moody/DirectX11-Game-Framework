#include "cube.h"
#include "idrawable.h"
#include "ibindable.h"

#include "material.h"
#include "meshprimitives.h"

#include "d3dapp.h"
#include "d3dutil.h"


namespace d3d
{
	Cube::Cube(D3DApp& app, Material* material)
		: m_transform{ {0.0f, 0.0f, 1.5f} },
		m_mesh{ app, MeshPrimitives::getCubeVertices(), MeshPrimitives::getCubeIndices() },
		m_material{ material }
	{
		DB_LOG("Constructing Cube");

		m_transform.rotate(0.0f, -10.0f, 0.0f);
	}


	void Cube::draw(D3DApp& app)
	{
		app.getScene().setWorldMatrix(app, m_transform.getTransformMatrix());

		m_material->bind(app);

		//m_mesh.draw(app);
	}


	void Cube::update(D3DApp& app, float deltaTime)
	{
		const float deltaSeconds = 0.001f * deltaTime;

		const float xRot = 0.0f * deltaSeconds;
		const float yRot = 15.0f * deltaSeconds;
		const float zRot = 0.0f * deltaSeconds;

		m_transform.rotate(xRot, yRot, zRot);
	}
}
