#pragma once

#include "vertex.h"
#include "skeletondata.h"
#include "animationdata.h"

#include <vector>
#include <string>
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

		const SkeletonData* getSkeleton(size_t index) const;

		const AnimationData* getAnimation(const std::string& name) const;

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

		const aiScene* m_scene{};

		std::vector<MeshData> m_meshData;
		std::vector<SkeletonData> m_skeletons;
		std::unordered_map<std::string, AnimationData> m_animations;
	};
}
