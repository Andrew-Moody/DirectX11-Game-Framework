#pragma once

#include "vertex.h"

#include <vector>
#include <unordered_map>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiBone;

namespace d3d
{
	class D3DApp;
	class Mesh;

	class ModelData
	{
	public:

		Mesh getMesh(D3DApp& app, size_t index);

		ModelData(const aiScene* scene);

		ModelData() = default;

	private:

		void processNode(const aiNode* node, const aiScene* scene);

		void processMesh(const aiMesh* mesh, const aiNode* meshNode, const aiScene* scene);

		void processBoneNode(const aiNode* node, const std::unordered_map<const aiNode*, const aiBone*>& skeletonNodes);

		struct MeshData
		{
			std::vector<Vertex> Vertices;
			std::vector<uint16_t> Indices;
		};

		std::vector<MeshData> m_meshData;
	};
}
