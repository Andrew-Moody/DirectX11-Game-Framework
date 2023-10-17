#pragma once

namespace d3d
{
	class D3DApp;

	class IBindable
	{
	public:

		virtual void bind(D3DApp& app) = 0;

		virtual inline ~IBindable() = 0;
	};

	IBindable::~IBindable() = default;
}
