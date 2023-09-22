#pragma once

#include <string>


struct aiScene;
struct aiNode;
struct aiMesh;

namespace d3d
{
	class ModelData;

	class AssetLoader
	{
	public:

		ModelData loadModel(const std::string& filePath);

	private:
		
	};
}
