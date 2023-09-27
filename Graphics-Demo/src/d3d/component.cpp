#include "component.h"

#include "gameobject.h"
#include "transform.h"

namespace d3d
{
	Component::Component(GameObject& gameObject)
		: m_gameObject{ gameObject }, m_transform{ gameObject.getTransform() }
	{

	}
}
