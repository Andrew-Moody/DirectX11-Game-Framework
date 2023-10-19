#pragma once

#include <DirectXMath.h>

#include <vector>
#include <string>
#include <unordered_map>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiBone;


namespace d3d
{
	struct Vertex;
	class AnimationData;

	class SkeletonData
	{
	public:

		void getFinalTransforms(float timeInSeconds,
			const AnimationData* animation,
			std::vector<DirectX::XMFLOAT4X4>& finalTransforms) const;

		void populateBoneWeights(std::vector<Vertex>& vertices);
		
		SkeletonData(const aiMesh* mesh, const aiNode* meshNode, const aiScene* scene);

	private:

		void processNode(const aiNode* node, int parentIdx);

		void printBones();

		void printOffsets();

		void printNodeTransforms();

		std::unordered_map<const aiNode*, const aiBone*> m_skeletonNodes;

		std::vector<std::string> m_boneNames;

		std::vector<int> m_parentIndices;

		std::vector<DirectX::XMFLOAT4X4> m_offsetTransforms;

		std::vector<const aiBone*> m_bones;

		std::vector<const aiNode*> m_nodes;
	};
}
