#pragma once

#include "iserializable.h"
#include "idrawable.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertex.h"

#include <vector>

namespace d3d
{
	class D3DApp;

	class Mesh : public ISerializable, public IDrawable
	{
	public:

		void draw(D3DApp& app) override;

		void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* elment) override;

		//void update(D3DApp& app, float deltaTime) override;

		Mesh(D3DApp& app, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);

		Mesh(D3DApp& app, std::vector<Vertex>&& vertices, std::vector<uint16_t>&& indices);

		Mesh() = default;

	private:

		std::vector<Vertex> m_vertices{};
		std::vector<uint16_t> m_indices{};

		VertexBuffer m_vertexBuffer{};
		IndexBuffer m_indexBuffer{};
	};
}
