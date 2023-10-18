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
	std::vector<GameObject*> ResourceManager::loadSceneXML(D3DApp& app, const std::string& path)
	{
		std::vector<GameObject*> gameObjects{};

		using namespace tinyxml2;

		tinyxml2::XMLDocument doc{};
		tinyxml2::XMLError error = doc.LoadFile(path.c_str());

		if (error)
		{
			std::cout << doc.ErrorStr() << '\n';
		}

		tinyxml2::XMLElement* root = doc.RootElement();

		if (!root)
		{
			std::cout << "Failed to find Root" << '\n';
		}

		XMLElement* current = root->FirstChildElement();

		while (current)
		{
			loadResource(app, current);
			
			if (strcmp(current->Name(), "GameObject") == 0)
			{
				GameObject* gameObject = dynamic_cast<GameObject*>(getResource(current->Attribute("id")));

				if (gameObject)
				{
					gameObjects.push_back(gameObject);
				}
			}

			current = current->NextSiblingElement();
		}


		// GameObjects are currently constructed in order from children to parent
		// Reverse the vector so that gameObjects are ordered from parent to child

		std::reverse(gameObjects.begin(), gameObjects.end());

		return gameObjects;
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
