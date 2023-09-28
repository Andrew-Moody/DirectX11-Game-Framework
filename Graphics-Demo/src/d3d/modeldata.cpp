#include "modeldata.h"

#include "d3dapp.h"
#include "vertex.h"
#include "mesh.h"


#include <assimp/scene.h>

#include <vector>
#include <unordered_map>
#include <queue>


namespace d3d
{
	Mesh ModelData::getMesh(D3DApp& app, size_t index)
	{
		if (index < m_meshData.size())
		{
			return Mesh(app, m_meshData[index].Vertices, m_meshData[index].Indices);
		}
		
		return Mesh();
	}


	ModelData::ModelData(const aiScene* scene)
	{
		processNode(scene->mRootNode, scene);
	}


	void ModelData::processNode(const aiNode* node, const aiScene* scene)
	{
		for (size_t i = 0; i < node->mNumMeshes; ++i)
		{

			processMesh(scene->mMeshes[node->mMeshes[i]], node, scene);
		}


		// Process child nodes
		for (size_t i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);
		}
	}


	void ModelData::processMesh(const aiMesh* mesh, const aiNode* meshNode, const aiScene* scene)
	{
		std::cout << "Mesh: " << mesh->mName.C_Str() << ", Node: " << meshNode->mName.C_Str() << '\n' << '\n';

		m_meshData.push_back(MeshData());

		std::vector<Vertex>& vertices{ m_meshData.back().Vertices};
		vertices.reserve(mesh->mNumVertices);
		
		for (size_t i = 0; i < mesh->mNumVertices; ++i)
		{
			aiVector3D vertex = mesh->mVertices[i];

			aiVector3D texCoords = mesh->mTextureCoords[0][i];

			vertices.push_back({ vertex.x, vertex.y, vertex.z, texCoords.x, texCoords.y});
		}

		std::vector<UINT16>& indices{ m_meshData.back().Indices};
		indices.reserve(static_cast<size_t>(mesh->mNumFaces * 3));

		for (size_t face_idx = 0; face_idx < mesh->mNumFaces; ++face_idx)
		{
			aiFace face = mesh->mFaces[face_idx];

			for (size_t i = 0; i < face.mNumIndices; ++i)
			{
				indices.push_back(static_cast<UINT16>(face.mIndices[i]));
			}
		}

		

		if (mesh->HasBones())
		{
			// populate the set of nodes that make up the skeleton
			std::unordered_map<const aiNode*, const aiBone*> skeletonNodes;

			for (size_t i = 0; i < mesh->mNumBones; ++i)
			{
				const aiNode* boneNode = scene->mRootNode->FindNode(mesh->mBones[i]->mName);

				skeletonNodes[boneNode] = mesh->mBones[i];

				// Follow the chain of parents up to the mesh (or meshes parent if sibling)
				// This is neccessary if the node hierarchy contains nodes that aren't bones
				while (boneNode != meshNode && boneNode != meshNode->mParent)
				{
					// mark a node as being a parent of a bone
					if (skeletonNodes.find(boneNode) == skeletonNodes.end())
					{
						skeletonNodes[boneNode] = nullptr;
					}

					boneNode = boneNode->mParent;
				}
			}

			// If the first child of mesh is a bone it is most likely the skeleton root
			// otherwise search from the parent of mesh until the first bone is found

			const aiNode* root{ meshNode->mChildren[0] };

			auto iter = skeletonNodes.find(root);

			if (iter == skeletonNodes.end() || !iter->second)
			{
				root = meshNode->mParent;

				std::queue<aiNode*> nodesToCheck{};

				nodesToCheck.push(meshNode->mParent);

				while (!nodesToCheck.empty())
				{
					aiNode* current = nodesToCheck.front();
					nodesToCheck.pop();

					std::cout << "Checking: " << current->mName.C_Str() << '\n';

					iter = skeletonNodes.find(current);

					if (iter != skeletonNodes.end() && iter->second)
					{
						root = current;
						break;
					}

					for (size_t i = 0; i < current->mNumChildren; ++i)
					{
						std::cout << "Pushing: " << current->mChildren[i]->mName.C_Str() << '\n';

						nodesToCheck.push(current->mChildren[i]);
					}
				}
			}

			//processBoneNode(meshNode->mParent, skeletonNodes);

			// This won't catch IK bones but thats fine now
			processBoneNode(root, skeletonNodes);

			std::cout << '\n';
		}
	}


	void ModelData::processBoneNode(const aiNode* node, const std::unordered_map<const aiNode*, const aiBone*>& skeletonNodes)
	{
		static int indent{};

		for (int i = 0; i < indent; ++i)
		{
			std::cout << "|  ";
		}

		auto iter = skeletonNodes.find(node);

		if (iter != skeletonNodes.end() && iter->second)
		{
			std::cout << "Bone: " << iter->second->mName.C_Str() << '\n';
		}
		else
		{
			std::cout << "Node: " << node->mName.C_Str() << '\n';
		}

		indent++;

		for (size_t i = 0; i < node->mNumChildren; ++i)
		{
			processBoneNode(node->mChildren[i], skeletonNodes);
		}

		indent--;
	}
}
