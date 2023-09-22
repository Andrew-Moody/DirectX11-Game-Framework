#pragma once

#include "idrawable.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

#include <vector>

namespace d3d
{
	class D3DApp;

	class Mesh : public IDrawable
	{
	public:

		void draw(D3DApp& app) override;

		void update(D3DApp& app, float deltaTime) override;

		Mesh(D3DApp& app, std::vector<Vertex> vertices, std::vector<UINT16> indices);

		Mesh() = default;

	private:

		VertexBuffer m_vertexBuffer{};
		IndexBuffer m_indexBuffer{};
	};
}
