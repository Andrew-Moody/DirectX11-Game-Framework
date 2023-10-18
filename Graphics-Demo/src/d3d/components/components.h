#pragma once

#include <string>
#include <map>
#include <memory>
#include <functional>

namespace d3d
{
	class Component;
	class GameObject;

	class Components
	{
	public:

		static std::unique_ptr<Component> CreateComponent(GameObject& gameObject, const std::string& name);

	private:

		static std::map<std::string, std::function<std::unique_ptr<Component>(GameObject&)>> m_factoryFunctions;
	};
}
