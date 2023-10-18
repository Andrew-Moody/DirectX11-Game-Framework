#include "components.h"

#include "spin.h"
#include "animationtest.h"

#include <iostream>

namespace d3d
{
	std::unique_ptr<Component> Components::CreateComponent(GameObject& gameObject, const std::string& name)
	{
		auto iter = m_factoryFunctions.find(name);

		if (iter != m_factoryFunctions.end())
		{
			return iter->second(gameObject);
		}

		std::cout << "Failed to create component with name: " << name << '\n';
		return nullptr;
	}


	std::map<std::string, std::function<std::unique_ptr<Component>(GameObject&)>> Components::m_factoryFunctions = {
		{ "Spin", [](GameObject& gameObject) { return std::make_unique<Spin>(gameObject); } },
		{ "AnimationTest", [](GameObject& gameObject) { return std::make_unique<AnimationTest>(gameObject); } },
	};
}
