#include "modeldata.h"

#include "d3dapp.h"
#include "vertex.h"
#include "mesh.h"
#include "skeletondata.h"
#include "mathutil.h"

#include <assimp/scene.h>
#include <DirectXMath.h>

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>



namespace d3d
{
	using namespace DirectX;

	Mesh ModelData::getMesh(D3DApp& app, size_t index)
	{
		if (index < m_meshData.size())
		{
			return Mesh(app, m_meshData[index].Vertices, m_meshData[index].Indices);
		}
		
		return Mesh();
	}

	const SkeletonData* ModelData::getSkeleton(size_t index) const
	{
		return index < m_skeletons.size() ? &m_skeletons[index] : nullptr;
	}


	const AnimationData* ModelData::getAnimation(const std::string& name) const
	{
		auto iter = m_animations.find(name);

		return iter != m_animations.end() ? &iter->second : nullptr;
	}

	ModelData::ModelData(const aiScene* scene)
		: m_scene{ scene }
	{
		std::string axisLabels[]{ "X", "Y", "Z" };
		const char signLabels[3] = { '-', '*', ' ' };

		// Print information about the scene to help debug coordinate system issues

		int upAxis = 0;
		scene->mMetaData->Get("UpAxis", upAxis);

		int upAxisSign = 0;
		scene->mMetaData->Get("UpAxisSign", upAxisSign);

		int forwardAxis = 0;
		scene->mMetaData->Get("FrontAxis", forwardAxis);

		int forwardAxisSign = 0;
		scene->mMetaData->Get("FrontAxisSign", forwardAxisSign);

		int rightAxis = 0;
		scene->mMetaData->Get("CoordAxis", rightAxis);

		int rightAxisSign = 0;
		scene->mMetaData->Get("CoordAxisSign", rightAxisSign);

		std::cout << "Up Axis: "  << signLabels[upAxisSign + 1] << axisLabels[upAxis] << '\n';
		std::cout << "Forward Axis: " << signLabels[forwardAxisSign + 1] << axisLabels[forwardAxis] << '\n';
		std::cout << "Right Axis: " << signLabels[rightAxisSign + 1] << axisLabels[rightAxis] << '\n';
		std::cout << '\n';


		std::cout << "Scene transform:" << '\n';
		std::cout << "SceneRoot name: " << scene->mRootNode->mName.C_Str() << '\n';
		printMatrix(scene->mRootNode->mTransformation);
		printDecomp(scene->mRootNode->mTransformation);
		std::cout << '\n';


		/*XMFLOAT4X4 test{};

		XMStoreFloat4x4(&test, XMMatrixRotationRollPitchYaw(1.0f, 2.0f, 3.0f));

		std::cout << "Decomposition Test" << '\n';
		printMatrix(test);
		printDecomp(test);
		std::cout << '\n';*/



		processNode(scene->mRootNode, scene);

		if (scene->HasAnimations())
		{
			for (size_t i = 0; i < scene->mNumAnimations; ++i)
			{
				const aiAnimation* anim = scene->mAnimations[i];

				std::cout << "Animation: " << anim->mName.C_Str() << '\n';

				m_animations.emplace(std::make_pair(anim->mName.C_Str(), AnimationData(anim)));
			}
		}
	}


	void ModelData::processNode(const aiNode* node, const aiScene* scene)
	{
		/*std::cout << "Process Node: " << node->mName.C_Str() << '\n';
		printMatrix(node->mTransformation);
		printDecomp(node->mTransformation);
		std::cout << '\n';*/

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
		
		//std::cout << "vertex positions" << '\n';

		for (size_t i = 0; i < mesh->mNumVertices; ++i)
		{
			const aiVector3D* pos = &mesh->mVertices[i];

			const aiVector3D* nor = &mesh->mNormals[i];

			const aiVector3D* tex = &mesh->mTextureCoords[0][i];

			const aiVector3D* tan = mesh->mTangents ? &mesh->mTangents[i] : nullptr;

			XMFLOAT3 position(pos->x, pos->y, pos->z);

			XMFLOAT3 normal(nor->x, nor->y, nor->z);

			XMFLOAT2 texCoord(tex->x, tex->y);

			XMFLOAT4 tangent(0.0f, 0.0f, 0.0f, 1.0f);

			if (tan)
			{
				tangent = XMFLOAT4(tan->x, tan->y, tan->z, 1.0f);
			}

			// Will have to set weights and bone indices later when processing the bones

			vertices.push_back({ position, normal, texCoord, tangent,
				{ 0.0f, 0.0f, 0.0f, 0.0f }, { 0, 0, 0, 0 } }
			);

			/*vertices.push_back({ position, normal, texCoord, tangent,
				{ 0.0f, 0.0f, 0.0f, 0.0f }, { 0, 0, 0, 0 }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0, 0, 0, 0 } }
			);*/

			
			//std::cout << "x: " << vertices[i].Position.x << ", y: " << vertices[i].Position.y << ", z: " << vertices[i].Position.z << '\n' << '\n';
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
			m_skeletons.push_back(SkeletonData(mesh, meshNode, scene));

			m_skeletons.back().populateBoneWeights(vertices);
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

		if (iter != skeletonNodes.end())
		{
			
			if (iter->second)
			{
				// Node is a bone
				std::cout << "Bone: " << iter->second->mName.C_Str() << '\n';
			}
			else
			{
				// Node is not a bone but is a part of the skeleton structure
				std::cout << "Node: " << node->mName.C_Str() << '\n';
			}
		}

		indent++;

		for (size_t i = 0; i < node->mNumChildren; ++i)
		{
			processBoneNode(node->mChildren[i], skeletonNodes);
		}

		indent--;
	}
}
