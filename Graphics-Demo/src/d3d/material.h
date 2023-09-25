#pragma once
#include "ibindable.h"

#include "vertexshader.h"
#include "pixelshader.h"

namespace d3d
{
	class D3DApp;
	class InputLayout;
	class VertexShader;
	class PixelShader;

	class Material : public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		Material(VertexShader* vertexShader, PixelShader* pixelShader);

		Material() = default;

	private:

		VertexShader* m_vertexShader{};

		PixelShader* m_pixelShader{};
	};
}
