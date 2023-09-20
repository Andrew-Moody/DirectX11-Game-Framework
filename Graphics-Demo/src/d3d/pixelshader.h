#pragma once
#include "ibindable.h"

#include <wrl/client.h>

struct ID3D11PixelShader;

namespace d3d
{
	class PixelShader : public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		PixelShader(D3DApp& app, const wchar_t* filepath);

		PixelShader() = default;

	private:

		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	};
}