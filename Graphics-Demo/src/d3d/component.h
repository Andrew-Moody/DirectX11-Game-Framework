#pragma once
#include "iserializable.h"

#include <string>

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

		void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element) override;

		const std::string& getType() { return m_type; }

		Component(GameObject& gameObject);

		virtual ~Component() = 0;

	protected:

		std::string m_type;

		GameObject& m_gameObject;
		Transform& m_transform;
	};


	inline Component::~Component() = default;
}
