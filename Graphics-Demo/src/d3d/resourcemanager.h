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


	class ResourceManager
	{
	public:

		void loadScene(D3DApp& app, const std::string& path);

		void loadSceneXML(D3DApp& app, const std::string& path);

		void loadResource(D3DApp& app, const tinyxml2::XMLElement* element);

		ISerializable* getResource(const std::string& id) const;

		AssetLoader& getAssetLoader() { return m_assetLoader; }

		ResourceManager();

		/*InputLayout* getInputLayout() { return m_inputLayout.get(); }

		Material* getMaterial() { return m_material.get(); }

		Mesh* getMesh() { return m_mesh.get(); }

		Mesh* getCubeMesh() { return m_cubeMesh.get(); }

		Texture* getTexture() { return m_texture.get(); }

		SamplerState* getSamplerState() { return m_samplerState.get(); }*/

		ModelData* getModel() { return m_model.get(); }

		//IBindable* getBindable(const std::string& name) const;

		Mesh* getMesh(const std::string& name) const;

	private:

		AssetLoader m_assetLoader{};

		std::map<std::string, std::function<std::unique_ptr<ISerializable>()>> m_factoryFunctions;

		std::map<std::string, std::unique_ptr<ISerializable>> m_resources;

		/*std::unique_ptr<InputLayout> m_inputLayout{};
		std::unique_ptr<VertexShader> m_vertexShader{};
		std::unique_ptr<PixelShader> m_pixelShader{};
		std::unique_ptr<SamplerState> m_samplerState{};
		std::unique_ptr<Texture> m_texture{};
		std::unique_ptr<Material> m_material{};
		std::unique_ptr<Mesh> m_mesh{};
		std::unique_ptr<Mesh> m_cubeMesh{};*/

		std::map<std::string, std::unique_ptr<IBindable>> m_bindables;

		std::map<std::string, std::unique_ptr<Mesh>> m_meshes;

		std::unique_ptr<ModelData> m_model{};
	};
}
