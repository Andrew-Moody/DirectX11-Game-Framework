#pragma once

#include <d3d11.h>

namespace d3d
{
	class IDrawable
	{
	public:

		virtual void draw(ID3D11DeviceContext& context) = 0;

		virtual inline ~IDrawable() = 0;
	};

	IDrawable::~IDrawable() = default;
}
