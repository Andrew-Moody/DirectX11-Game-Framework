#include "staticmeshrenderer.h"

#include "../material.h"
#include "../mesh.h"
#include "../modeldata.h"

#include "../d3dapp.h"
#include "../d3dutil.h"

#include <tinyxml2.h>

#include <iostream>

namespace d3d
{
	void StaticMeshRenderer::draw(D3DApp& app)
	{
		app.getScene().setWorldMatrix(app, m_transform.getTransformMatrix());

		m_material->bind(app);

		m_mesh->draw(app);
	}


	void StaticMeshRenderer::update(float deltaTime)
	{

	}


	void StaticMeshRenderer::deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element)
	{
		using namespace tinyxml2;

		DB_ASSERT(strcmp(element->Attribute("type"), "StaticMeshRenderer") == 0);

		const XMLElement* materialElement = element->FirstChildElement("Material");

		const XMLElement* modelElement = element->FirstChildElement("Model");

		if (!materialElement)
		{
			std::cout << "Material missing from gameObject: " << element->Attribute("id") << '\n';
			return;
		}

		if (!modelElement)
		{
			std::cout << "Model missing from gameObject: " << element->Attribute("id") << '\n';
			return;
		}

		m_material = dynamic_cast<Material*>(app.getResourceManager().getResource(materialElement->Attribute("id")));

		if (!m_material)
		{
			std::cout << "Missing Material for StaticMeshRender: " << element->Attribute("id") << '\n';
			return;
		}

		ModelData* model = dynamic_cast<ModelData*>(app.getResourceManager().getResource(modelElement->Attribute("id")));

		if (model)
		{
			m_mesh = model->getMesh(0);
		}

		if (!m_mesh)
		{
			std::cout << "Missing Mesh for StaticMeshRender: " << element->Attribute("id") << '\n';
			return;
		}
	}
}
