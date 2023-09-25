#pragma once
#include "ibindable.h"

#include <wrl/client.h>

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace d3d
{
	class D3DApp;
	struct TextureData;
	

	class Texture : IBindable
	{
	public:

		void bind(D3DApp& app) override;

		Texture(D3DApp& app, const TextureData& textureData);

	private:

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;
	};
}
