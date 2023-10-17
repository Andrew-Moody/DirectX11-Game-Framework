#pragma once
#include "ibindable.h"
#include "iserializable.h"


namespace d3d
{
	class D3DApp;
	class InputLayout;
	class VertexShader;
	class PixelShader;
	class Texture;
	class SamplerState;

	class Material : public ISerializable, public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element) override;

		Material(VertexShader* vertexShader, PixelShader* pixelShader);

		Material() = default;

	private:

		InputLayout* m_inputLayout{};

		VertexShader* m_vertexShader{};

		PixelShader* m_pixelShader{};

		Texture* m_texture{};

		SamplerState* m_samplerState{};
	};
}
