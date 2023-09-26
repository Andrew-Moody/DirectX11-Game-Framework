#pragma once

namespace d3d
{
	class GameObject;
	class Transform;

	class Component
	{
	public:

		virtual void update(float deltaTime) = 0;

		Component(GameObject& gameObject, Transform& transform);

		virtual inline ~Component() = 0;

	protected:

		GameObject& m_gameObject;
		Transform& m_transform;
	};


	Component::~Component() = default;
}
