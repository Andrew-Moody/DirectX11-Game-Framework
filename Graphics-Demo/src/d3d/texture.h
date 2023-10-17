#pragma once
#include "ibindable.h"
#include "iserializable.h"

#include <wrl/client.h>

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace d3d
{
	class D3DApp;
	struct TextureData;
	

	class Texture : public ISerializable, public IBindable
	{
	public:

		void bind(D3DApp& app) override;

		void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element) override;

		Texture(D3DApp& app, const TextureData& textureData);

		Texture() = default;

	private:

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;
	};
}
