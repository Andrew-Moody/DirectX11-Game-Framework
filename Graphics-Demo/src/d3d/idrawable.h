#pragma once

namespace d3d
{
	class D3DApp;

	class IDrawable
	{
	public:

		virtual void draw(D3DApp& app) = 0;

		virtual inline ~IDrawable() = 0;
	};

	IDrawable::~IDrawable() = default;
}
