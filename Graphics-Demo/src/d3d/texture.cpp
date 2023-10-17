#include "texture.h"

#include "texturedata.h"
#include "d3dapp.h"
#include "d3dutil.h"

#include <tinyxml2.h>

namespace d3d
{
	void Texture::bind(D3DApp& app)
	{
		DB_LOG("Binding Texture");

		// This cost over an hour
		//app.getContext().PSGetShaderResources(0u, 1u, m_textureView.GetAddressOf());

		app.getContext().PSSetShaderResources(0u, 1u, m_textureView.GetAddressOf());
	}


	void Texture::deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element)
	{
		TextureData textureData = app.getResourceManager().getAssetLoader().loadTexture(app, element->Attribute("id"));

		D3D11_TEXTURE2D_DESC texDesc{};
		texDesc.Width = static_cast<UINT>(textureData.width);
		texDesc.Height = static_cast<UINT>(textureData.height);
		texDesc.MipLevels = 1u;
		texDesc.ArraySize = 1u;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1u;
		texDesc.SampleDesc.Quality = 0u;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0u;
		texDesc.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA subResourceData{};
		subResourceData.pSysMem = textureData.data.get();
		subResourceData.SysMemPitch = static_cast<UINT>(textureData.width * 4);

		HR(app.getDevice().CreateTexture2D(&texDesc, &subResourceData, &m_texture));


		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc{};
		viewDesc.Format = texDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip = 0;
		viewDesc.Texture2D.MipLevels = 1;

		HR(app.getDevice().CreateShaderResourceView(m_texture.Get(), &viewDesc, &m_textureView));
	}


	Texture::Texture(D3DApp& app, const TextureData& textureData)
	{
		D3D11_TEXTURE2D_DESC texDesc{};
		texDesc.Width = static_cast<UINT>(textureData.width);
		texDesc.Height = static_cast<UINT>(textureData.height);
		texDesc.MipLevels = 1u;
		texDesc.ArraySize = 1u;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1u;
		texDesc.SampleDesc.Quality = 0u;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0u;
		texDesc.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA subResourceData{};
		subResourceData.pSysMem = textureData.data.get();
		subResourceData.SysMemPitch = static_cast<UINT>(textureData.width * 4);

		HR(app.getDevice().CreateTexture2D(&texDesc, &subResourceData, &m_texture));


		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc{};
		viewDesc.Format = texDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip = 0;
		viewDesc.Texture2D.MipLevels = 1;

		HR(app.getDevice().CreateShaderResourceView(m_texture.Get(), &viewDesc, &m_textureView));
	}
}
