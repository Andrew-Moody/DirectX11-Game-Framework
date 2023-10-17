#include "mesh.h"

#include "d3dapp.h"

#include "vertex.h"

namespace d3d
{
	void Mesh::draw(D3DApp& app)
	{
		app.getContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_vertexBuffer.bind(app);

		m_indexBuffer.bind(app);

		app.getContext().DrawIndexed(static_cast<UINT>(m_indexBuffer.getIndexCount()), 0u, 0u);
	}


    void Mesh::deserializeXML(D3DApp& app, const tinyxml2::XMLElement* elment)
    {
		
    }


	/*void Mesh::update(D3DApp& app, float deltaTime)
	{

	}*/


	Mesh::Mesh(D3DApp& app, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
		: m_vertices{vertices}, m_indices{indices}, m_vertexBuffer(app, vertices, 0u), m_indexBuffer(app, indices)
	{

	}


	Mesh::Mesh(D3DApp& app, std::vector<Vertex>&& vertices, std::vector<uint16_t>&& indices)
		: m_vertices{std::move(vertices)},
		m_indices{std::move(indices)},
		m_vertexBuffer(app, m_vertices, 0u),
		m_indexBuffer(app, m_indices)
	{

	}
}
