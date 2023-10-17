#include "resourcemanager.h"

#include "modeldata.h"
#include "texturedata.h"

#include "bytecode.h"
#include "inputlayout.h"
#include "vertexshader.h"
#include "pixelshader.h"
#include "material.h"
#include "texture.h"
#include "gameobject.h"

#include "meshprimitives.h"
#include "d3dutil.h"
#include <d3d11.h>
#include <tinyxml2.h>

#include <fstream>
#include <string>
#include <iostream>

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
			{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT,  0u, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"TexCoord", 0u, DXGI_FORMAT_R32G32_FLOAT,  0u, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"Tangent", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT,  0u, 32u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"Weights", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT,  0u, 48u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			{"BoneIndices", 0u, DXGI_FORMAT_R8G8B8A8_UINT,  0u, 64u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			//{"WeightsX", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT,  0u, 68u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
			//{"BoneIndicesX", 0u, DXGI_FORMAT_R8G8B8A8_UINT,  0u, 84u, D3D11_INPUT_PER_VERTEX_DATA, 0u}
			//{"Color", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, Xu, D3D11_INPUT_PER_VERTEX_DATA, 0u}
			};

			//m_inputLayout = std::make_unique<InputLayout>(app, elementDesc, vsByteCode);
			m_bindables["InputLayout"] = std::make_unique<InputLayout>(app, elementDesc, vsByteCode);

			//m_vertexShader = std::make_unique<VertexShader>(app, vsByteCode);
			m_bindables["VertexShader"] = std::make_unique<VertexShader>(app, vsByteCode);

			//m_pixelShader = std::make_unique<PixelShader>(app, psByteCode);
			m_bindables["PixelShader"] = std::make_unique<PixelShader>(app, psByteCode);

			//m_material = std::make_unique<Material>(m_vertexShader.get(), m_pixelShader.get());
			m_bindables["Material"] = std::make_unique<Material>(
				dynamic_cast<VertexShader*>(m_bindables["VertexShader"].get()),
				dynamic_cast<PixelShader*>(m_bindables["PixelShader"].get())
			);


			std::string texturePath;

			file >> ignore >> texturePath;

			DB_LOG("Loading Texture: " << texturePath);

			TextureData texData = m_assetLoader.loadTexture(app, texturePath);
			
			//m_bindables["Texture"] = std::move(m_texture);
			m_bindables["Texture"] = std::make_unique<Texture>(app, texData);

			//m_samplerState = std::make_unique<SamplerState>(app);
			m_bindables["SamplerState"] = std::make_unique<SamplerState>(app);
		}


		for (size_t i = 0; i < numMeshes; ++i)
		{
			std::string modelPath;

			file >> ignore >> modelPath;

			DB_LOG("Loading Model: " << modelPath);

			m_model = std::make_unique<ModelData>(m_assetLoader.loadModel(app, modelPath));

			//m_mesh = std::make_unique<Mesh>(m_model->getMesh(app, 0u));
			//m_meshes["Man"] = std::make_unique<Mesh>(m_model->getMesh(app, 0u));

			/*if (m_meshes.count(modelPath) < 1)
			{
				ModelData modelData = m_assetLoader.loadModel(modelPath);

				m_meshes[modelPath] = std::make_unique<Mesh>(modelData.getMesh(app, 0u));
			}*/
		}

		//m_cubeMesh = std::make_unique<Mesh>(app, MeshPrimitives::getCubeVertices(), MeshPrimitives::getCubeIndices());
	}


	void ResourceManager::loadSceneXML(D3DApp& app, const std::string& path)
	{
		using namespace tinyxml2;

		tinyxml2::XMLDocument doc{};
		tinyxml2::XMLError error = doc.LoadFile("./src/scene.xml");

		if (error)
		{
			std::cout << doc.ErrorStr() << '\n';
		}

		tinyxml2::XMLElement* root = doc.RootElement();

		if (root)
		{
			std::cout << "Root was not null" << '\n';

			tinyxml2::XMLNode* child = root->FirstChild();

			if (child)
			{
				std::cout << "Child was not null" << '\n';

				std::cout << child->Value() << '\n';
			}
		}

		XMLElement* current = root->FirstChildElement();

		while (current)
		{
			loadResource(app, current);

			current = current->NextSiblingElement();
		}

	}


	void ResourceManager::loadResource(D3DApp& app, const tinyxml2::XMLElement* element)
	{
		std::cout << "Loading Resource: " << element->Name() << ", id: " << element->Attribute("id") << "\n";

		if (m_resources.find(element->Attribute("id")) != m_resources.end())
		{
			std::cout << "Element already loaded with id: " << element->Attribute("id") << '\n';
			return;
		}

		auto iter = m_factoryFunctions.find(element->Name());

		if (iter == m_factoryFunctions.end())
		{
			std::cout << "No factory function found for element type: " << element->Name() << '\n';
			return;
		}

		std::unique_ptr<ISerializable> resource = iter->second();

		resource->deserializeXML(app, element);

		m_resources[element->Attribute("id")] = std::move(resource);
	}


	ISerializable* ResourceManager::getResource(const std::string& name) const
	{
		auto iter = m_resources.find(name);

		if (iter != m_resources.end())
		{
			return iter->second.get();
		}

		return nullptr;
	}


	Mesh* ResourceManager::getMesh(const std::string& name) const
	{
		auto iter = m_meshes.find(name);

		if (iter != m_meshes.end())
		{
			return iter->second.get();
		}

		return nullptr;
	}


	ResourceManager::ResourceManager()
	{
		m_factoryFunctions["InputLayout"] = []() { return std::make_unique<InputLayout>(); };
		m_factoryFunctions["VertexShader"] = []() { return std::make_unique<VertexShader>(); };
		m_factoryFunctions["PixelShader"] = []() { return std::make_unique<PixelShader>(); };
		m_factoryFunctions["SamplerState"] = []() { return std::make_unique<SamplerState>(); };
		m_factoryFunctions["Texture"] = []() { return std::make_unique<Texture>(); };
		m_factoryFunctions["Material"] = []() { return std::make_unique<Material>(); };
		m_factoryFunctions["Model"] = []() { return std::make_unique<ModelData>(); };
		m_factoryFunctions["GameObject"] = []() { return std::make_unique<GameObject>(); };
	}
}
