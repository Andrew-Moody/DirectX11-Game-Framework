#include "gameobject.h"

#include "transform.h"
#include "mesh.h"
#include "material.h"

#include "components/spin.h"

#include "d3dapp.h"

#include <iostream>

namespace d3d
{
	void GameObject::draw(D3DApp& app)
	{
		app.getScene().setWorldMatrix(app, m_transform.getTransformMatrix());

		m_material->bind(app);

		m_mesh->draw(app);
	}


	void GameObject::update(D3DApp& app, float deltaTime)
	{
		for (auto& component : m_components)
		{
			component->update(deltaTime);
		}
	}


	GameObject::GameObject(D3DApp& app, Mesh* mesh, Material* material)
		: m_transform{}, m_mesh{ mesh }, m_material{ material }
	{
		m_components.push_back(std::make_unique<Spin>(*this, m_transform));
	}
}
