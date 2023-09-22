#include "mesh.h"

#include "d3dapp.h"

#include "vertex.h"

namespace d3d
{
	void Mesh::draw(D3DApp& app)
	{
		m_vertexBuffer.bind(app);

		m_indexBuffer.bind(app);

		app.getContext().DrawIndexed(m_indexBuffer.getIndexCount(), 0u, 0u);
	}


	void Mesh::update(D3DApp& app, float deltaTime)
	{

	}


	Mesh::Mesh(D3DApp& app, std::vector<Vertex> vertices, std::vector<UINT16> indices)
		: m_vertexBuffer(app, vertices, 0u), m_indexBuffer(app, indices)
	{

	}
}
