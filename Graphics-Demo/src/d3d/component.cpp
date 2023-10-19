#include "component.h"

#include "gameobject.h"
#include "transform.h"

#include <tinyxml2.h>

namespace d3d
{
	void Component::deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element)
	{
		m_type = element->Attribute("type");
	}


	Component::Component(GameObject& gameObject)
		: m_gameObject{ gameObject }, m_transform{ gameObject.getTransform() }
	{

	}
}
