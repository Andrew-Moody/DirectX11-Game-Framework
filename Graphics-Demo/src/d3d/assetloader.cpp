#include "assetloader.h"

#include "modeldata.h"
#include "d3dutil.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <string>


namespace d3d
{
	ModelData AssetLoader::loadModel(const std::string& filePath)
	{
		Assimp::Importer importer;

		constexpr unsigned int flags{ aiProcess_Triangulate | aiProcess_ConvertToLeftHanded };

		const aiScene* scene = importer.ReadFile(filePath, flags);

		if (!scene)
		{
			DB_LOG(importer.GetErrorString());
			return ModelData{};
		}

		return ModelData{ scene };
	}
}
