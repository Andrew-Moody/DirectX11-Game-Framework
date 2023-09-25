#pragma once
#include "ibindable.h"

#include "assetloader.h"
#include "mesh.h"

#include "inputlayout.h"
#include "vertexshader.h"
#include "pixelshader.h"
#include "samplerstate.h"
#include "texture.h"
#include "material.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace d3d
{
	class D3DApp;


	class ResourceManager
	{
	public:

		void loadScene(D3DApp& app, const std::string& path);

		InputLayout* getInputLayout() { return m_inputLayout.get(); }

		Material* getMaterial() { return m_material.get(); }

		Mesh* getMesh() { return m_mesh.get(); }

		Texture* getTexture() { return m_texture.get(); }

		SamplerState* getSamplerState() { return m_samplerState.get(); }


	private:

		AssetLoader m_assetLoader{};

		std::unique_ptr<InputLayout> m_inputLayout{};
		std::unique_ptr<VertexShader> m_vertexShader{};
		std::unique_ptr<PixelShader> m_pixelShader{};
		std::unique_ptr<SamplerState> m_samplerState{};
		std::unique_ptr<Texture> m_texture{};
		std::unique_ptr<Material> m_material{};
		std::unique_ptr<Mesh> m_mesh{};

		//std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshes;
	};
}
