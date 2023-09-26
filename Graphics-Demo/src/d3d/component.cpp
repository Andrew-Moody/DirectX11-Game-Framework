#include "component.h"

#include "gameobject.h"
#include "transform.h"

namespace d3d
{
	Component::Component(GameObject& gameObject, Transform& transform)
		: m_gameObject{ gameObject }, m_transform{ transform }
	{

	}
}
