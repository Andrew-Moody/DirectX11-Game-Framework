#include "material.h"

#include "vertexshader.h"
#include "pixelshader.h"

#include "d3dapp.h"
#include "d3dutil.h"

#include <tinyxml2.h>

#include <iostream>

namespace d3d
{
	using namespace tinyxml2;

	void Material::bind(D3DApp& app)
	{
		m_inputLayout->bind(app);
		
		m_vertexShader->bind(app);

		m_pixelShader->bind(app);

		if (m_texture)
		{
			m_texture->bind(app);
		}

		if (m_samplerState)
		{
			m_samplerState->bind(app);
		}
	}


	void Material::deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element)
	{
		DB_ASSERT(strcmp(element->Name(), "Material") == 0);

		const XMLElement* ilElement = element->FirstChildElement("InputLayout");

		const XMLElement* vsElement = element->FirstChildElement("VertexShader");

		const XMLElement* psElement = element->FirstChildElement("PixelShader");

		const XMLElement* textureElement = element->FirstChildElement("Texture");

		const XMLElement* samplerElement = element->FirstChildElement("SamplerState");

		if (!ilElement)
		{
			std::cout << "XML definition for Material was missing definition for InputLayout" << '\n';
			return;
		}

		std::cout << "InputLayout id: " << ilElement->Attribute("id") << '\n';

		if (!vsElement)
		{
			std::cout << "XML definition for Material was missing definition for VertexShader" << '\n';
			return;
		}

		std::cout << "VertexShader path: " << vsElement->Attribute("id") << '\n';

		if (!psElement)
		{
			std::cout << "XML definition for Material was missing definition for PixelShader" << '\n';
			return;
		}

		std::cout << "PixelShader path: " << psElement->Attribute("id") << '\n';


		m_inputLayout = dynamic_cast<InputLayout*>(app.getResourceManager().getResource(ilElement->Attribute("id")));

		m_vertexShader = dynamic_cast<VertexShader*>(app.getResourceManager().getResource(vsElement->Attribute("id")));

		m_pixelShader = dynamic_cast<PixelShader*>(app.getResourceManager().getResource(psElement->Attribute("id")));

		m_texture = dynamic_cast<Texture*>(app.getResourceManager().getResource(textureElement->Attribute("id")));

		m_samplerState = dynamic_cast<SamplerState*>(app.getResourceManager().getResource(samplerElement->Attribute("id")));
	}


	Material::Material(VertexShader* vertexShader, PixelShader* pixelShader)
		: m_vertexShader{ vertexShader }, m_pixelShader{ pixelShader }
	{

	}
}
