#include "resourcemanager.h"

#include "modeldata.h"
#include "texturedata.h"

#include "bytecode.h"
#include "inputlayout.h"
#include "vertexshader.h"
#include "pixelshader.h"
#include "material.h"
#include "d3dutil.h"

#include <d3d11.h>

#include <fstream>
#include <string>

namespace d3d
{
	void ResourceManager::loadScene(D3DApp& app, const std::string& path)
	{
		std::ifstream file{ path };

		std::string ignore;

		if (!file)
		{
			DB_LOG("Failed to open scene file" << '\n' << path);
			return;
		}

		DB_LOG("Loading Scene: " << path);

		size_t numMaterials{};
		size_t numMeshes{};

		file >> ignore >> numMaterials;
		file >> ignore >> numMeshes;

		DB_LOG("NumMaterials: " << numMaterials);
		DB_LOG("NumMeshes: " << numMeshes);

		for (size_t i = 0; i < numMaterials; ++i)
		{
			std::string vsShaderPath;
			std::string psShaderPath;

			file >> ignore >> vsShaderPath;
			file >> ignore >> psShaderPath;

			//load
			DB_LOG("Loading VertexShader: " << vsShaderPath);
			DB_LOG("Loading PixelShader: " << psShaderPath);

			std::vector<uint8_t> vsByteCode = CreateShaderByteCode(vsShaderPath);

			std::vector<uint8_t> psByteCode = CreateShaderByteCode(psShaderPath);

			std::vector<D3D11_INPUT_ELEMENT_DESC> elementDesc{
			{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"TexCoord", 0u, DXGI_FORMAT_R32G32_FLOAT,  0u, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			//{"Color", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, 20u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
			};

			m_inputLayout = std::make_unique<InputLayout>(InputLayout(app, elementDesc, vsByteCode));

			m_vertexShader = std::make_unique<VertexShader>(VertexShader(app, vsByteCode));

			m_pixelShader = std::make_unique<PixelShader>(PixelShader(app, psByteCode));

			m_material = std::make_unique<Material>(Material(m_vertexShader.get(), m_pixelShader.get()));


			std::string texturePath;

			file >> ignore >> texturePath;

			DB_LOG("Loading Texture: " << texturePath);

			TextureData texData = m_assetLoader.loadTexture(texturePath);

			m_texture = std::make_unique<Texture>(Texture(app, texData));

			m_samplerState = std::make_unique<SamplerState>(SamplerState(app));
		}


		for (size_t i = 0; i < numMeshes; ++i)
		{
			std::string modelPath;

			file >> ignore >> modelPath;

			DB_LOG("Loading Model: " << modelPath);

			ModelData modelData = m_assetLoader.loadModel(modelPath);

			m_mesh = std::make_unique<Mesh>(modelData.getMesh(app, 0u));

			/*if (m_meshes.count(modelPath) < 1)
			{
				ModelData modelData = m_assetLoader.loadModel(modelPath);

				m_meshes[modelPath] = std::make_unique<Mesh>(modelData.getMesh(app, 0u));
			}*/
		}
	}
}
