#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "ibindable.h"

namespace d3d
{
	class PixelShader : public IBindable
	{
	public:

		void bind(ID3D11DeviceContext& context) override;

		PixelShader(ID3D11Device& device, const wchar_t* filepath);

		PixelShader() = default;

	private:

		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	};
}