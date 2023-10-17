#pragma once

namespace tinyxml2
{
	class XMLElement;
}

namespace d3d
{
	class D3DApp;

	class ISerializable
	{
	public:

		virtual void deserializeXML(D3DApp& app, const tinyxml2::XMLElement* element) = 0;

		virtual ~ISerializable() = 0;
	};

	inline ISerializable::~ISerializable() = default;
}
