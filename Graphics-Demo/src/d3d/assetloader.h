#pragma once

#include <string>
#include <vector>


struct aiScene;
struct aiNode;
struct aiMesh;

namespace d3d
{
	class ModelData;
	struct TextureData;

	class AssetLoader
	{
	public:

		ModelData loadModel(const std::string& filePath);

		TextureData loadTexture(const std::string& filePath);

		//ShaderData loadShader(const std::string& filePath);

	private:
		
	};
}
