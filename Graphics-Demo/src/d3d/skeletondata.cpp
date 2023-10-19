#include "skeletondata.h"

#include "vertex.h"
#include "animationdata.h"

#include "mathutil.h"

#include "assimp/scene.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <iostream>

namespace d3d
{
	using namespace DirectX;

	void SkeletonData::getFinalTransforms(float timeInSeconds,
		const AnimationData* animation,
		std::vector<DirectX::XMFLOAT4X4>& finalTransforms) const
	{
		// Seems there should be away to reduce the amount of storing and loading
		// to/from XMMATRIX. A bit tricky without using a vector of XMMATRIX (16-byte alignment issues)

		if (finalTransforms.size() < m_boneNames.size())
		{
			finalTransforms.resize(m_boneNames.size());
		}


		// get the interpolated transform for each bone to its parent
		for (size_t i = 0; i < m_boneNames.size(); ++i)
		{
			animation->interpolateBone(timeInSeconds, m_boneNames[i], finalTransforms[i]);
		}

		//std::cout << "ToParentTransform for bone: " << m_boneNames[0] << '\n';
		//printMatrix(finalTransforms[0]);
		//printDecomp(finalTransforms[0]);
		//std::cout << '\n';


		// traverse the skeleton applying accumulated parent transforms to root
		// parentIndices is ordered such that parents always appear before their children
		for (size_t i = 1; i < m_parentIndices.size(); ++i)
		{
			/*std::cout << "ToParentTransform for bone: " << m_boneNames[i] << '\n';
			printMatrix(finalTransforms[i]);
			printDecomp(finalTransforms[i]);
			std::cout << '\n';


			std::cout << "ParentToRootTransform for bone: " << m_boneNames[i] << '\n';
			printMatrix(finalTransforms[m_parentIndices[i]]);
			printDecomp(finalTransforms[m_parentIndices[i]]);
			std::cout << '\n';*/


			// The transform of the current bone to its parent
			XMMATRIX toParent = XMLoadFloat4x4(&finalTransforms[i]);
			
			// The transform of the current bones parent to the root
			XMMATRIX parentToRoot = XMLoadFloat4x4(&finalTransforms[m_parentIndices[i]]);

			// Calculate the toRoot transform for this bone
			XMStoreFloat4x4(&finalTransforms[i],
				XMMatrixMultiply(toParent, parentToRoot)
			);

			// Possible one of the most time spent on a typo so far
			// finalTransforms[m_parentIndices[i]] used when finalTransforms[i] should have been
		}

		/*for (size_t i = 0; i < m_boneNames.size(); ++i)
		{
			std::cout << "ToRootTransform for bone: " << m_boneNames[i] << '\n';
			printMatrix(finalTransforms[i]);
			printDecomp(finalTransforms[i]);
			std::cout << '\n';
		}*/


		// Since I copy/pasted the offset loop from the parent loop I forget to change
		// the loop start from 1 back to 0

		// Apply the offset transform. Can't be done before parent to root transformation
		for (size_t i = 0; i < m_offsetTransforms.size(); ++i)
		{
			// offset transform of the current bone
			XMMATRIX offset = XMLoadFloat4x4(&m_offsetTransforms[i]);

			// transform of the current bone to root calculated in the previous step
			XMMATRIX toRoot = XMLoadFloat4x4(&finalTransforms[i]);

			// apply the offset to get the final transform
			XMStoreFloat4x4(&finalTransforms[i],
				XMMatrixMultiply(offset, toRoot)
			);
		}


		//for (size_t i = 0; i < m_boneNames.size(); ++i)
		//{
		//	std::cout << "FinalTransform for bone: " << m_boneNames[i] << '\n';
		//	//printMatrix(finalTransforms[i]);
		//	printDecomp(finalTransforms[i]);
		//	std::cout << '\n';
		//}

		//std::cout << "FinalTransform for bone: " << m_boneNames[0] << '\n';
		//printMatrix(finalTransforms[0]);
		//printDecomp(finalTransforms[0]);
		//std::cout << '\n';
	}


	void SkeletonData::populateBoneWeights(std::vector<Vertex>& vertices)
	{
		std::map<size_t, std::vector<std::pair<size_t, float>>> weights;


		for (size_t boneIdx = 0; boneIdx < m_bones.size(); ++boneIdx)
		{
			size_t numWeights = m_bones[boneIdx]->mNumWeights;

			//std::cout << "Bone weights for: " << m_boneNames[boneIdx] << " with index: " << boneIdx << '\n';

			for (size_t i = 0; i < numWeights; ++i)
			{
				const aiVertexWeight& weight = m_bones[boneIdx]->mWeights[i];

				weights[weight.mVertexId].push_back(std::make_pair(boneIdx, weight.mWeight));

				//std::cout << "Vertex: " << weight.mVertexId << ", Weight: " << weight.mWeight << '\n';
			}
		}

		std::vector<size_t> verticesGreaterThan4Weights{};

		for (const auto& iter : weights)
		{
			//if (iter.second.size() > 4)
			//{
			//	std::cout << "Warning: vertex was influenced by more than 4 bones. num bone weights: " << iter.second.size()<< '\n';
			//}
			//else if (iter.second.size() < 4)
			//{
			//	// Pad the weights with zero to make things easier
			//	for (size_t i = iter.second.size(); i < 4; i++)
			//	{
			//		weights[iter.first].push_back(std::make_pair(0, 0.0f));
			//	}
			//}

			//vertices[iter.first].BoneIndices[0] = iter.second[0].first;
			//vertices[iter.first].BoneIndices[1] = iter.second[1].first;
			//vertices[iter.first].BoneIndices[2] = iter.second[2].first;
			//vertices[iter.first].BoneIndices[3] = iter.second[3].first;

			//vertices[iter.first].Weights.x = iter.second[0].second;
			//vertices[iter.first].Weights.y = iter.second[1].second;
			//vertices[iter.first].Weights.z = iter.second[2].second;


			if (iter.second.size() > 4)
			{
				std::cout << "Warning: vertex " << iter.first << " was influenced by more than 4 bones. num bone weights: "
					<< iter.second.size() << '\n';

				verticesGreaterThan4Weights.push_back(iter.first);
			}

			if (iter.second.size() > 4)
			{
				std::cout << "Warning: vertex was influenced by more than 8 bones. num bone weights: "
					<< iter.second.size() << '\n';
			}
			else if (iter.second.size() < 4)
			{
				// Pad the weights with zero to make things easier
				for (size_t i = iter.second.size(); i < 8; i++)
				{
					weights[iter.first].push_back(std::make_pair(0, 0.0f));
				}
			}

			//if (iter.second.size() > 8)
			//{
			//	std::cout << "Warning: vertex was influenced by more than 8 bones. num bone weights: "
			//		<< iter.second.size() << '\n';
			//}
			//else if (iter.second.size() < 8)
			//{
			//	// Pad the weights with zero to make things easier
			//	for (size_t i = iter.second.size(); i < 8; i++)
			//	{
			//		weights[iter.first].push_back(std::make_pair(0, 0.0f));
			//	}
			//}

			vertices[iter.first].BoneIndices[0] = iter.second[0].first;
			vertices[iter.first].BoneIndices[1] = iter.second[1].first;
			vertices[iter.first].BoneIndices[2] = iter.second[2].first;
			vertices[iter.first].BoneIndices[3] = iter.second[3].first;

			/*vertices[iter.first].BoneIndices2[0] = iter.second[4].first;
			vertices[iter.first].BoneIndices2[1] = iter.second[5].first;
			vertices[iter.first].BoneIndices2[2] = iter.second[6].first;
			vertices[iter.first].BoneIndices2[3] = iter.second[7].first;*/

			vertices[iter.first].Weights.x = iter.second[0].second;
			vertices[iter.first].Weights.y = iter.second[1].second;
			vertices[iter.first].Weights.z = iter.second[2].second;
			vertices[iter.first].Weights.w = iter.second[3].second;

			normalizeWeights(vertices[iter.first].Weights);

			/*vertices[iter.first].Weights.w = 1.0f - vertices[iter.first].Weights.x
				- vertices[iter.first].Weights.y - vertices[iter.first].Weights.z;*/

			/*vertices[iter.first].Weights2.x = iter.second[4].second;
			vertices[iter.first].Weights2.y = iter.second[5].second;
			vertices[iter.first].Weights2.z = iter.second[6].second;
			vertices[iter.first].Weights2.w = iter.second[7].second;*/
		}


		for (size_t i = 0; i < verticesGreaterThan4Weights.size(); ++i)
		{
			const Vertex& vert = vertices[verticesGreaterThan4Weights[i]];

			

			//std::cout << "Vertex: " << verticesGreaterThan4Weights[i]
			//	<< " (" << m_boneNames[vert.BoneIndices[0]] << "," << vert.Weights.x
			//	<< "), (" << m_boneNames[vert.BoneIndices[1]] << "," << vert.Weights.y
			//	<< "), (" << m_boneNames[vert.BoneIndices[2]] << "," << vert.Weights.z
			//	<< "), (" << m_boneNames[vert.BoneIndices[3]] << "," << vert.Weights.w
			//	/*<< "), (" << m_boneNames[vert.BoneIndices2[0]] << "," << vert.Weights2.x
			//	<< "), (" << m_boneNames[vert.BoneIndices2[1]] << "," << vert.Weights2.y
			//	<< "), (" << m_boneNames[vert.BoneIndices2[2]] << "," << vert.Weights2.z
			//	<< "), (" << m_boneNames[vert.BoneIndices2[3]] << "," << vert.Weights2.w*/
			//	<< ")" << '\n';
		}

		std::cout << '\n';

		for (size_t i = 0; i < vertices.size(); ++i)
		{
			const Vertex& vert = vertices[i];

			float sum = vert.Weights.x + vert.Weights.y + vert.Weights.z + vert.Weights.w;

			//std::cout << "Vertex: " << i << " , sum: " << sum
			//	  << " (" << m_boneNames[vert.BoneIndices[0]] << "," << vert.Weights.x
			//	<< "), (" << m_boneNames[vert.BoneIndices[1]] << "," << vert.Weights.y
			//	<< "), (" << m_boneNames[vert.BoneIndices[2]] << "," << vert.Weights.z
			//	<< "), (" << m_boneNames[vert.BoneIndices[3]] << "," << vert.Weights.w
			//	/*<< "), (" << m_boneNames[vert.BoneIndices2[0]] << "," << vert.Weights2.x
			//	<< "), (" << m_boneNames[vert.BoneIndices2[1]] << "," << vert.Weights2.y
			//	<< "), (" << m_boneNames[vert.BoneIndices2[2]] << "," << vert.Weights2.z
			//	<< "), (" << m_boneNames[vert.BoneIndices2[3]] << "," << vert.Weights2.w*/
			//	<< ")" << '\n';
		}
	}


	void SkeletonData::processNode(const aiNode* node, int parentIdx)
	{
		// This won't catch IK bones but that's fine now

		// Check if the node is needed in the skeleton

		auto iter = m_skeletonNodes.find(node);

		if (iter != m_skeletonNodes.end())
		{
			const aiBone* bone = iter->second;

			int currentIdx = m_parentIndices.size();
			m_parentIndices.push_back(parentIdx);

			m_boneNames.push_back(bone->mName.C_Str());

			// go ahead and save the whole bone for now
			// to get the vertex weights later
			m_bones.push_back(bone);


			// Will likely need to transpose the result
			const aiMatrix4x4& mat = bone->mOffsetMatrix;
			XMFLOAT4X4 offset((const float*)&mat);

			// If not transposed the resulting XMMATRIX decomposition
			// does not detect the translation
			XMMATRIX matrix = XMLoadFloat4x4(&offset);
			XMStoreFloat4x4(&offset, XMMatrixTranspose(matrix));

			// Test matrix conversion from assimp to directX
			/*std::cout << "Test Matrix Conversion for bone: " << bone->mName.C_Str() << '\n';
			printMatrix(mat);
			printDecomp(mat);
			std::cout << '\n';
			printMatrix(offset);
			printDecomp(offset);
			std::cout << '\n';*/

			// Decompositions seem to match when transposing
			// so keeping it for now


			m_offsetTransforms.push_back(offset);

			for (size_t i = 0; i < node->mNumChildren; ++i)
			{
				processNode(node->mChildren[i], currentIdx);
			}
		}


		// Save the node even if not a bone for the transformation matrix
		m_nodes.push_back(node);
	}


	void SkeletonData::printBones()
	{
		// Use parent index lookup to determine indent level
		for (size_t boneIdx = 0; boneIdx < m_parentIndices.size(); ++boneIdx)
		{
			int i = boneIdx;

			while (i > 0)
			{
				std::cout << "|   ";

				i = m_parentIndices[i];
			}

			std::cout << m_boneNames[boneIdx] << '\n';
		}
	}


	void SkeletonData::printOffsets()
	{
		for (size_t i = 0; i < m_offsetTransforms.size(); ++i)
		{
			/*std::cout << "Bone offset for: " << m_boneNames[i] << '\n';
			printMatrix(m_offsetTransforms[i]);
			printDecomp(m_offsetTransforms[i]);
			std::cout << '\n';*/
		}

		std::cout << "Bone offset for: " << m_boneNames[0] << '\n';
		printMatrix(m_offsetTransforms[0]);
		printDecomp(m_offsetTransforms[0]);
		std::cout << '\n';
	}


	void SkeletonData::printNodeTransforms()
	{
		for (size_t i = 0; i < m_nodes.size(); ++i)
		{
			/*std::cout << "Node transformation for: " << m_nodes[i]->mName.C_Str() << '\n';
			printMatrix(m_nodes[i]->mTransformation);
			printDecomp(m_nodes[i]->mTransformation);
			std::cout << '\n';*/
		}

		std::cout << "Node transformation for: " << m_nodes[0]->mName.C_Str() << '\n';
		printMatrix(m_nodes[0]->mTransformation);
		printDecomp(m_nodes[0]->mTransformation);
		std::cout << '\n';
	}


	SkeletonData::SkeletonData(const aiMesh* mesh, const aiNode* meshNode, const aiScene* scene)
	{
		// populate the set of nodes that make up the skeleton
		for (size_t i = 0; i < mesh->mNumBones; ++i)
		{
			const aiNode* boneNode = scene->mRootNode->FindNode(mesh->mBones[i]->mName);

			m_skeletonNodes[boneNode] = mesh->mBones[i];

			//std::cout << mesh->mBones[i]->mName.C_Str() << '\n';

			// Follow the chain of parents up to the mesh (or meshes parent if sibling)
			// This is neccessary if the node hierarchy contains nodes that aren't bones
			while (boneNode != meshNode && boneNode != meshNode->mParent)
			{
				// mark a node as being a parent of a bone
				if (m_skeletonNodes.find(boneNode) == m_skeletonNodes.end())
				{
					m_skeletonNodes[boneNode] = nullptr;
				}

				boneNode = boneNode->mParent;
			}
		}

		// If the first child of mesh is a bone it is most likely the skeleton root
		// otherwise search from the parent of mesh until the first bone is found

		

		const aiNode* root{};

		if (meshNode->mNumChildren > 0)
		{
			root = meshNode->mChildren[0];
		}

		auto iter = m_skeletonNodes.find(root);

		if (iter == m_skeletonNodes.end() || !iter->second)
		{
			root = meshNode->mParent;

			std::queue<aiNode*> nodesToCheck{};

			nodesToCheck.push(meshNode->mParent);

			while (!nodesToCheck.empty())
			{
				aiNode* current = nodesToCheck.front();
				nodesToCheck.pop();

				//std::cout << "Checking: " << current->mName.C_Str() << '\n';

				iter = m_skeletonNodes.find(current);

				if (iter != m_skeletonNodes.end() && iter->second)
				{
					root = current;
					break;
				}

				for (size_t i = 0; i < current->mNumChildren; ++i)
				{
					//std::cout << "Pushing: " << current->mChildren[i]->mName.C_Str() << '\n';

					nodesToCheck.push(current->mChildren[i]);
				}
			}
		}


		processNode(root, -1);

		printBones();

		std::cout << '\n';

		printOffsets();

		std::cout << '\n';

		printNodeTransforms();

		std::cout << '\n';
	}
}
