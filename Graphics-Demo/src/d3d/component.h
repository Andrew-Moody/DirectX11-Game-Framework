#pragma once

#include "iserializable.h"

namespace tinyxml2
{
	class XMLElement;
}

namespace d3d
{
	class GameObject;
	class Transform;

	class Component : public ISerializable
	{
	public:

		virtual void update(float deltaTime) = 0;

		void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element) override = 0;

		Component(GameObject& gameObject);

		virtual ~Component() = 0;

	protected:

		GameObject& m_gameObject;
		Transform& m_transform;
	};


	inline Component::~Component() = default;
}
