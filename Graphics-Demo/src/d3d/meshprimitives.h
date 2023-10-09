#pragma once

#include "vertex.h"

#include <vector>

namespace d3d
{
	class MeshPrimitives
	{
	public:

		static const std::vector<SimpleVertex>& getCubeVertices()
		{
			return s_cubeVertices;
		}


		static const std::vector<uint16_t>& getCubeIndices()
		{
			return s_cubeIndices;
		}

	private:

		static const std::vector<SimpleVertex> s_cubeVertices;

		static const std::vector<uint16_t> s_cubeIndices;
	};
}
