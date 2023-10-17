#pragma once
#include "iserializable.h"
#include "vertex.h"
#include "mesh.h"
#include "skeletondata.h"
#include "animationdata.h"

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiBone;

namespace d3d
{
	class D3DApp;

	class ModelData : public ISerializable
	{
	public:

		void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element) override;

		Mesh* getMesh(size_t index) const;

		const SkeletonData* getSkeleton(size_t index) const;

		const AnimationData* getAnimation(const std::string& name) const;

		ModelData(D3DApp& app, const aiScene* scene);

		ModelData() = default;

	private:

		void processNode(const aiNode* node, const aiScene* scene);

		void processMesh(const aiMesh* mesh, const aiNode* meshNode, const aiScene* scene);

		void processBoneNode(const aiNode* node, const std::unordered_map<const aiNode*, const aiBone*>& skeletonNodes);

		/*struct MeshData
		{
			std::vector<Vertex> Vertices;
			std::vector<uint16_t> Indices;
		};*/

		D3DApp* m_app;

		const aiScene* m_scene{};

		//std::vector<MeshData> m_meshData;

		std::vector<std::unique_ptr<Mesh>> m_meshes;
		std::vector<SkeletonData> m_skeletons;
		std::unordered_map<std::string, AnimationData> m_animations;
	};
}
