#pragma once

#include <d3d11.h>

namespace d3d
{
	class IBindable
	{
	public:

		virtual void bind(ID3D11DeviceContext& context) = 0;

		virtual inline ~IBindable() = 0;
	};

	IBindable::~IBindable() = default;
}
