#pragma once
#include "ibindable.h"
#include "iserializable.h"

#include "assetloader.h"
#include "mesh.h"

#include "inputlayout.h"
#include "vertexshader.h"
#include "pixelshader.h"
#include "samplerstate.h"
#include "texture.h"
#include "material.h"
#include "modeldata.h"

#include <memory>
#include <string>
#include <map>
#include <functional>

namespace d3d
{
	class D3DApp;
	class GameObject;


	class ResourceManager
	{
	public:

		std::vector<GameObject*> loadSceneXML(D3DApp& app, const std::string& path);

		void loadResource(D3DApp& app, const tinyxml2::XMLElement* element);

		ISerializable* getResource(const std::string& id) const;

		AssetLoader& getAssetLoader() { return m_assetLoader; }

		ResourceManager();

	private:

		AssetLoader m_assetLoader{};

		std::map<std::string, std::function<std::unique_ptr<ISerializable>()>> m_factoryFunctions;

		std::map<std::string, std::unique_ptr<ISerializable>> m_resources;
	};
}
