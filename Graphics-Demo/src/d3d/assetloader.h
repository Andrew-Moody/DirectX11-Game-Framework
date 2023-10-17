#pragma once

#include <string>
#include <vector>


struct aiScene;
struct aiNode;
struct aiMesh;

namespace d3d
{
	class D3DApp;
	class ModelData;
	struct TextureData;

	class AssetLoader
	{
	public:

		ModelData loadModel(D3DApp& app, const std::string& filePath) const;

		TextureData loadTexture(D3DApp& app, const std::string& filePath) const;

	private:
		
	};
}
