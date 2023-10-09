#pragma once
#include "../component.h"
#include "../transform.h"

#include <iostream>

namespace d3d
{
	class GameObject;
	class Transform;


	class Spin : public Component
	{
	public:

		void update(float deltaTime) override
		{
			const float deltaSeconds = 0.001f * deltaTime;

			const float xRot = 0.0f * deltaSeconds;
			const float yRot = 15.0f * deltaSeconds;
			const float zRot = 0.0f * deltaSeconds;

			m_transform.rotate(xRot, yRot, zRot);
		}


		Spin(GameObject& gameObject)
			: Component(gameObject)
		{
			
		}
	};
}
