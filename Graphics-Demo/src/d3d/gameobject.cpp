#include "gameobject.h"

#include "transform.h"
#include "idrawable.h"
#include "components/components.h"

#include "d3dapp.h"
#include "d3dutil.h"

#include <tinyxml2.h>

#include <vector>
#include <iostream>


namespace d3d
{
	void GameObject::deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element)
	{
		using namespace tinyxml2;

		m_app = &app;

		DB_ASSERT(strcmp(element->Name(), "GameObject") == 0);

		m_id = element->Attribute("id");

		const XMLElement* posElement = element->FirstChildElement("Position");

		if (posElement)
		{
			float x = posElement->FloatAttribute("x");
			float y = posElement->FloatAttribute("y");
			float z = posElement->FloatAttribute("z");

			m_transform.translate(x, y, z);
		}


		const XMLElement* componentsElement = element->FirstChildElement("Components");

		if (componentsElement)
		{
			const XMLElement* current = componentsElement->FirstChildElement();

			while (current)
			{
				std::unique_ptr<Component> component = Components::CreateComponent(*this, current->Attribute("type"));

				component->Component::deserializeXML(app, current);

				component->deserializeXML(app, current);

				m_components.push_back(std::move(component));

				current = current->NextSiblingElement();
			}

			// Attempt to find a drawable component
			for (auto& component : m_components)
			{
				IDrawable* drawable = dynamic_cast<IDrawable*>(component.get());

				if (drawable)
				{
					if (m_drawable)
					{
						std::cout << "GameObject " << m_id << " has multiple drawable components" << '\n';
					}

					m_drawable = drawable;
				}
			}
		}

		const XMLElement* childrenElement = element->FirstChildElement("ChildObjects");

		if (childrenElement)
		{
			const XMLElement* current = childrenElement->FirstChildElement();

			while (current)
			{
				GameObject* child = dynamic_cast<GameObject*>(app.getResourceManager().getResource(current->Attribute("id")));

				if (child)
				{
					child->setParent(this);
				}

				current = current->NextSiblingElement();
			}
		}
	}


	void GameObject::draw(D3DApp& app)
	{
		if (m_drawable)
		{
			m_drawable->draw(app);
		}
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

		/*for (auto& child : m_children)
		{
			child->update(app, deltaTime);
		}*/
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


	Component* GameObject::getComponent(const std::string& type)
	{
		for (auto& component : m_components)
		{
			if (component->getType() == type)
			{
				return component.get();
			}
		}

		return nullptr;
	}


	GameObject::GameObject(D3DApp& app, Mesh* mesh, Material* material)
		: m_app{ &app }, m_transform {}, m_mesh{ mesh }, m_material{ material }
	{
		
	}
}
