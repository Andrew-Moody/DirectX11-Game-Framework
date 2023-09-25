#pragma once

#include "idrawable.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertex.h"

#include <vector>

namespace d3d
{
	class D3DApp;

	class Mesh : public IDrawable
	{
	public:

		void draw(D3DApp& app) override;

		void update(D3DApp& app, float deltaTime) override;

		Mesh(D3DApp& app, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);

		Mesh() = default;

		~Mesh() override = default;

	private:

		std::vector<Vertex> m_vertices{};
		std::vector<uint16_t> m_indices{};

		VertexBuffer m_vertexBuffer{};
		IndexBuffer m_indexBuffer{};
	};
}
