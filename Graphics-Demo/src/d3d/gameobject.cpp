#include "gameobject.h"

#include "transform.h"
#include "mesh.h"
#include "material.h"

#include "components/spin.h"

#include "d3dapp.h"

#include <vector>


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
		if (m_parent)
		{
			m_transform.updateParentTransform(m_parent->m_transform);
		}
		

		for (auto& component : m_components)
		{
			component->update(deltaTime);
		}

		for (auto& child : m_children)
		{
			child->update(app, deltaTime);
		}
	}


	void GameObject::setParent(GameObject* parent)
	{
		// Prevent multiple child entries
		if (parent == m_parent)
		{
			return;
		}

		// Remove previous parent child relationship
		if (m_parent)
		{
			auto iter = std::find(m_parent->m_children.begin(), m_parent->m_children.end(), this);

			if (iter != m_parent->m_children.end())
			{
				m_parent->m_children.erase(iter);
			}
		}

		// Set new parent child relationship
		m_parent = parent;

		if (m_parent)
		{
			m_parent->m_children.push_back(this);
		}
	}


	void GameObject::addComponent(std::unique_ptr<Component> component)
	{
		m_components.push_back(std::move(component));
	}


	GameObject::GameObject(D3DApp& app, Mesh* mesh, Material* material)
		: m_app{ app }, m_transform {}, m_mesh{ mesh }, m_material{ material }
	{
		
	}
}
