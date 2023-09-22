#include "modeldata.h"

#include "d3dapp.h"
#include "vertex.h"
#include "mesh.h"


#include <assimp/scene.h>

#include <vector>

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
			processMesh(scene->mMeshes[node->mMeshes[i]], scene);
		}


		// Process child nodes
		for (size_t i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);
		}
	}


	void ModelData::processMesh(const aiMesh* mesh, const aiScene* scene)
	{
		m_meshData.push_back(MeshData());

		std::vector<Vertex>& vertices{ m_meshData.back().Vertices};
		vertices.reserve(mesh->mNumVertices);
		
		for (size_t i = 0; i < mesh->mNumVertices; ++i)
		{
			aiVector3D vertex = mesh->mVertices[i];

			vertices.push_back({ vertex.x, vertex.y, vertex.z, 255u, 0u, 255u, 255u});
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
	}
}
